#pragma once

#include "stdafx.h"
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <vector>

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

#define D3DFVF_TLVERTEX  D3DFVF_XYZ | D3DFVF_DIFFUSE

struct TRANSLITVERTEX
{
	float x;
	float y;
	float z;
	DWORD color;
};

struct Particle
{
	float x;
	float y;
	float size;
	float xSpeed;
	float ySpeed;
	float xAccel;
	float yAccel;
	float r;
	float g;
	float b;
	float a;
	float alphaDecay;
	bool doDelete;

	Particle()
	{
		x = 0.0f;
		y = 0.0f;
		size = 0.0f;
		xSpeed = 0.0f;
		ySpeed = 0.0f;
		xAccel = 0.0f;
		yAccel = 0.0f;
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 0.0f;
		alphaDecay = 0.0f;
		doDelete = false;
	}

	void ResetColor(float red, float green, float blue, float alpha, float decay)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
		alphaDecay = decay;
	}


	void ResetIt(float x, float y, float xSpeed, float ySpeed, float xAccel, float yAccel, float size)
	{
		this->x = x;
		this->y = y;
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
		this->xAccel = xAccel;
		this->yAccel = yAccel;
		this->size = size;
	}

	void Update(float time)
	{
		x = x + xSpeed * time;
		y = y + ySpeed * time;

		xSpeed = xSpeed + xAccel * time;
		ySpeed = ySpeed + yAccel * time;

		a = a - alphaDecay * time;
	}
};

class CParticleSystem
{
protected:
	std::vector<TRANSLITVERTEX> vertexes;
	std::vector<Particle> particles;

	float x, y;
	float size;

	long prevTime;

	float tmp;

public:
	BOOL textureIsAlphaBlended;

	CParticleSystem(void) {textureIsAlphaBlended=TRUE;tmp=0.0f; x=0.0f; y=0.0f; size=0.0; prevTime=0;}
	~CParticleSystem(void){}

	float rnd()
	{
		return ((float) rand() / (float) RAND_MAX);
	}

	void Begin(size_t particleCount)
	{
		prevTime = timeGetTime();
		size = 20.0f;

		TRANSLITVERTEX v = {0};
		Particle p;
		for(size_t i = 0; i < particleCount; ++i)
		{
			Reset(p, i);
			particles.push_back(p);
			vertexes.push_back(v);
		}

		tmp = 10.0f;

	}

	void SetPos(float fx, float fy)
	{
		x = fx;
		y = fy;
	}


	void Reset(Particle& particle, size_t index)
	{
		float curX = x +  rnd() * 10.0f;
		float curY = y;

		//particle.ResetIt(curX, curY, 0, 0.0, 0.0, 0, 2.0f);
		//particle.ResetColor(1.0f, 1.0f, 1.0f, 1.0f, 0.25f);
		particle.ResetIt(curX, curY, -0.4f + (rnd() * 0.8f), -0.5f - (rnd() * 0.4f), 0, -(rnd() * 0.3f), 2.0f);
		particle.ResetColor(0.9f, 0.5f, 0.2f, 0.6f + (0.2f * rnd()), 0.01f + rnd() * 0.05f);
	}


	void AddSpark(float x, float y, float ySpeed, float yAccel=0.3f)
	{
		Particle p;
		TRANSLITVERTEX v = {0};
		v.z = 1.0f;
		Reset(p, particles.size());
		p.x = x +  rnd() * 10.0f;
		p.y = y + rnd() * 0.6f;
		p.yAccel = rnd() * -yAccel;
		p.ySpeed = ySpeed;

		p.doDelete = true;

		particles.push_back(p);
		vertexes.push_back(v);
	}

	void Update()
	{
		DWORD now = timeGetTime();
		float elapsed = float(now - prevTime) / 100.0f;
		prevTime = now;

		for(size_t i = 0; i < particles.size(); ++i)
		{
			Particle& particle = particles[i];
			particle.Update(elapsed);
			if(particle.a <= 0)
			{
				if(particle.doDelete)
				{
					particles.erase(particles.begin()+i);
					vertexes.erase(vertexes.begin()+i);
					--i;
					continue;
				}
				else
				{
					Reset(particle, i);
				}
			}
			
			vertexes[i].color = D3DCOLOR_COLORVALUE(particle.r, particle.g, particle.b, particle.a);
			vertexes[i].x = particle.x;
			vertexes[i].y = particle.y;
		}
	}

	void Render(IDirect3DDevice9* device)
	{
		HRESULT hRes = S_OK;

        device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		hRes = device->SetRenderState(D3DRS_POINTSPRITEENABLE, 1);
		hRes = device->SetRenderState(D3DRS_POINTSCALEENABLE, 0);
		hRes = device->SetRenderState(D3DRS_POINTSIZE, F2DW(size));
		hRes = device->SetRenderState(D3DRS_POINTSIZE_MIN, F2DW(0));
		hRes = device->SetRenderState(D3DRS_POINTSCALE_A, F2DW(0));
		hRes = device->SetRenderState(D3DRS_POINTSCALE_B, F2DW(0));
		hRes = device->SetRenderState(D3DRS_POINTSCALE_C, F2DW(1));
		//hRes = device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

		if(textureIsAlphaBlended)
		{
			hRes = device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA  );
			hRes = device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			hRes = device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			hRes = device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			hRes = device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			hRes = device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		}
		else
		{
			hRes = device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE  );
			hRes = device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			hRes = device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		}
			
		hRes = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		hRes = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		hRes = device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

		hRes = device->SetFVF(D3DFVF_TLVERTEX);
		hRes = device->DrawPrimitiveUP(D3DPT_POINTLIST, (UINT)vertexes.size(), &vertexes[0], sizeof(TRANSLITVERTEX));

		hRes = device->SetRenderState(D3DRS_POINTSPRITEENABLE, 0);
		hRes = device->SetRenderState(D3DRS_POINTSCALEENABLE, 0);
	}
};
