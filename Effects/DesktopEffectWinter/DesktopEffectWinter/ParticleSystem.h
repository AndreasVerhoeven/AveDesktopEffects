#pragma once

#pragma once

#include "stdafx.h"
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <vector>

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

#define D3DFVF_TLVERTEX  D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1

struct TRANSLITVERTEX
{
	float x;
	float y;
	float z;
	DWORD color;
	float u, v;
};

struct Particle
{
	float x;
	float y;
	float size;
	float sizeDelta;
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
	int type;

	float rotation;
	float rotationDelta;

	Particle()
	{
		rotation = 0.0f;
		rotationDelta = 0.0f;
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
		g = blue;
		a = alpha;
		alphaDecay = decay;
	}


	void ResetIt(float x, float y, float xSpeed, float ySpeed, float xAccel, float yAccel, float size, float sizeDelta)
	{
		this->x = x;
		this->y = y;
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
		this->xAccel = xAccel;
		this->yAccel = yAccel;
		this->size = size;
		this->sizeDelta = sizeDelta;
	}

	void Update(float time)
	{
		x = x + xSpeed * time;
		y = y + ySpeed * time;

		xSpeed = xSpeed + xAccel * time;
		ySpeed = ySpeed + yAccel * time;
		rotation = rotation + rotationDelta * time;

		a = a - alphaDecay * time;

		size = size + sizeDelta * time;
	}
};

struct Bucket
{
	float numFlakes;
	float decay;
	float left;
	float width;

	BOOL negativeImage;

	Bucket()
	{
		negativeImage = FALSE;
		left = 0.0f;
		width = 0.0f;
		numFlakes = 0.0f;
		decay = 0.0f;
	}

	void Update(float time)
	{

		numFlakes -= decay * time * (numFlakes / (10.0f*75.0f));

		if(numFlakes <= 0.0f)
			numFlakes = 0.0f;
	}

	void AddFlake(float weight)
	{
		if(numFlakes <= 65.0f)
			weight *= 1.50f;

		numFlakes += weight;
	}
};

struct SnowCoveredIcon
{
	int itemId;
	float amount;
	RECT rc;

	SnowCoveredIcon()
	{
		itemId = 0;
		amount = 0.0f;
		SetRect(&rc, 0, 0, 0, 0);
	}
};

class CParticleSystem
{
protected:
	std::vector<TRANSLITVERTEX> vertexes;
	std::vector<Particle> particles;
	std::vector<Bucket> buckets;

	std::vector<SnowCoveredIcon> icons;

	float x, y;
	float size;

	long prevTime;

	float tmp;
	float height;
	float width;
	int numTypes;

	float bucketWidth;

public:
	CParticleSystem(void) {bucketWidth=0.0f;width=0.0f;height=0.0f;tmp=0.0f; x=0.0f; y=0.0f; size=0.0; prevTime=0;}
	~CParticleSystem(void){}

	float rnd()
	{
		return ((float) rand() / (float) RAND_MAX);
	}

	void ClearBuckets()
	{
		buckets.clear();
	}

	void InitBuckets(int bucketWidth)
	{
		buckets.clear();
		float l = - float(bucketWidth) / 3.0f;
		this->bucketWidth = float(bucketWidth);
		int numBuckets = bucketWidth > 0  ? (int(width+bucketWidth*2)*2) / bucketWidth : 0;

		for(size_t i = 0; i < (size_t)numBuckets; ++i)
		{
			Bucket b;
			b.width = this->bucketWidth;
			b.left = float(i) * (b.width);
			b.decay  = 0.2f;
			b.width = this->bucketWidth;
			b.negativeImage = rnd() >= 0.5f;
			buckets.push_back(b);
		}

		for(size_t i = 0; i < (size_t)numBuckets; ++i)
		{
			Bucket b;
			b.width = this->bucketWidth;
			b.left = l + float(i) * (b.width) + (-10.0f + rnd() * 20.0f);
			b.decay  = 0.2f;
			b.width = this->bucketWidth;
			b.negativeImage = rnd() >= 0.5f;
			buckets.push_back(b);
		}
	}

	void Begin(int width, int height, int numTypes)
	{
		this->numTypes = numTypes;
		this->width  = float(width);
		this->height = float(height);
		prevTime = timeGetTime();
		size = 30.0f;
		tmp = 10.0f;

		srand((UINT)timeGetTime());

	}

	void SetPos(float fx, float fy)
	{
		x = fx;
		y = fy;
	}


	void AddSpark(float x, float y, float wind=0.0f, float speed=1.0f, float fsize=25.0f)
	{
		Particle p;
		TRANSLITVERTEX v = {0};
		v.z = 1.0f;
		
		float xSpeed = wind + rnd() * 1.4f * (rnd() > 0.5f ? 1.0f : -1.0f);
		float ySpeed = (speed/4.0f)+ rnd() * speed * 2.0f;
		float xAccel = 0.00001f;
		float yAccel = 0.005f + rnd() * (speed/75.0f);
		float size = 2.0f + rnd() * (fsize - 2.0f);
		float sizeDelta = rnd() > 0.6f ? -0.05f + rnd() * 0.1f : 0.0f;
		p.ResetIt(x, y, xSpeed, ySpeed, xAccel, yAccel, size, sizeDelta);
		p.ResetColor(1.0f, 1.0f, 1.0f, 1.0f, rnd() > 0.6f ? rnd() * 0.01f : 0.0f);
		p.doDelete = true;
		p.type = int(rnd() * (float(numTypes) - 0.001f));
		p.rotationDelta = -0.03f + rnd() * 0.06f;

		particles.push_back(p);
		vertexes.push_back(v);
	}

	void Update(HWND listview, float hitTestSizeTreshold)
	{
		DWORD now = timeGetTime();
		float elapsed = float(now - prevTime) / 100.0f;
		prevTime = now;

		for(size_t i = 0; i < particles.size(); ++i)
		{
			Particle& particle = particles[i];
			particle.Update(elapsed);

			if(listview != NULL && particle.size <= hitTestSizeTreshold)
			{
				POINT ptTestLeft = {int(particle.x + width),
									int(particle.y + height + particle.size/2.0f) };

				LVHITTESTINFO hit = {0};
				hit.pt = ptTestLeft;
				//hit.flags = LVHT_ONITEMICON;
				if(ListView_HitTest(listview, &hit) != -1 && hit.iItem >= 0 && hit.flags & LVHT_ONITEMICON &&
					!(hit.flags & LVHT_NOWHERE))
				{
					particles.erase(particles.begin()+i);
					vertexes.erase(vertexes.begin()+i);
					--i;
					continue;
				}
			}

			if(particle.y > height + particle.size /2)
			{
				
				
				if(buckets.size() > 0)
				{
					float left = particle.x + width;
					for(size_t i = 0; i < buckets.size(); ++i)
					{
						if(left >= buckets[i].left && left <= buckets[i].left + buckets[i].width)
							buckets[i].AddFlake(particle.size / 2.75f);
					}
					
				}

				particles.erase(particles.begin()+i);
				vertexes.erase(vertexes.begin()+i);
				--i;
				continue;
				
			}
			else if(particle.a <= 0 || particle.size <= 0)
			{
				particles.erase(particles.begin()+i);
				vertexes.erase(vertexes.begin()+i);
				--i;
				continue;
			}
			
			//vertexes[i].color = D3DCOLOR_COLORVALUE(particle.r, particle.g, particle.b, particle.a);
			//vertexes[i].x = particle.x;
			//vertexes[i].y = particle.y;
		}

		for(size_t i = 0; i < buckets.size(); ++i)
		{
			buckets[i].Update(elapsed);
		}
	}


	void Render(IDirect3DDevice9* device, float cx, float cy, IDirect3DTexture9** textures)
	{
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->LightEnable(0, FALSE);

		D3DXMATRIX Projection, Rotation, World;
		D3DXMatrixOrthoLH(&Projection, cx, -cy, 0.0f, 1.0f);
		device->SetTransform(D3DTS_PROJECTION, &Projection);


		for(size_t i = 0; i < particles.size(); ++i)
		{
			Particle& particle = particles[i];

			HRESULT hRes = S_OK;
			D3DXMatrixIdentity(&World);
			D3DXMatrixTranslation(&World, particle.x, particle.y, 0.0f);
			D3DXMatrixRotationZ(&Rotation, particle.rotation); 
			World = Rotation * World;
			device->SetTransform(D3DTS_WORLD, &World);

			device->SetTexture(0, textures[particle.type]);
			hRes = device->SetFVF(D3DFVF_TLVERTEX);
			DWORD color = D3DCOLOR_COLORVALUE(particle.r, particle.g, particle.b, particle.a);
			float psize = particle.size;
			TRANSLITVERTEX v[4] = {0};
			v[0].color = v[1].color = v[2].color = v[3].color = color;
			v[0].x = -psize /2.0f;
			v[0].y = psize /2.0f;
			v[0].u = 0.0f; v[0].v = 0.0f;

			v[1].x = psize /2.0f;
			v[1].y = psize /2.0f;
			v[1].u = 1.0f; v[1].v = 0.0f;

			v[2].x = psize /2.0f;
			v[2].y = -psize /2.0f;
			v[2].u = 1.0f; v[2].v = 1.0f;

			v[3].x = - psize /2.0f;
			v[3].y = - psize /2.0f;
			v[3].u = 0.0f; v[3].v = 1.0f;


			hRes = device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, (UINT)2, (void*)&v[0], sizeof(TRANSLITVERTEX));
		}
	}

	void RenderBuckets(IDirect3DDevice9* device, IDirect3DTexture9* ground)
	{
		for(size_t i = 0; i < buckets.size(); ++i)
			RenderBucket(i, device, ground);
	}

	void RenderBucket(size_t index, IDirect3DDevice9* device, IDirect3DTexture9* ground)
	{
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->LightEnable(0, FALSE);

		D3DXMATRIX Projection, Rotation, World;
		D3DXMatrixOrthoLH(&Projection, width*2, height*2, 0.0f, 1.0f);
		device->SetTransform(D3DTS_PROJECTION, &Projection);
		device->SetTexture(0, ground);

		Bucket& b = buckets[index];

		float left = index * bucketWidth;
		float right = left + bucketWidth;
		float width = bucketWidth;
		float height = 32.0f;

		if(b.negativeImage)
			width = -width;

		TRANSLITVERTEX v[4] = {0};
		DWORD color = 0xFFFFFFFF;
		v[0].color = v[1].color = v[2].color = v[3].color = color;
		v[0].x = -width /2.0f;
		v[0].y = height /2.0f;
		v[0].u = 0.0f; v[0].v = 0.2f;

		v[1].x = width /2.0f;
		v[1].y = height /2.0f;
		v[1].u = 1.0f; v[1].v = 0.2f;

		v[2].x = width /2.0f;
		v[2].y = -height /2.0f;
		v[2].u = 1.0f; v[2].v = 1.0f;

		v[3].x = - width /2.0f;
		v[3].y = - height /2.0f;
		v[3].u = 0.0f; v[3].v = 1.0f;

		if(b.negativeImage)
			width = -width;

		float start = 5.0f;
		float moveUp = 1.0f;
		float treshold = 75.0f;
		int numNeeded = min(int(b.numFlakes / treshold), 45);
		for(int i = 0; i < numNeeded; ++i)
		{
			D3DXMatrixIdentity(&World);
			D3DXMatrixTranslation(&World, -this->width + width / 2.0f + b.left, -this->height + height/2.0f + float(i) * moveUp - start, 0.0f);
			device->SetTransform(D3DTS_WORLD, &World);
			device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, (UINT)2, (void*)&v[0], sizeof(TRANSLITVERTEX));
		}

		float toRender = b.numFlakes - treshold * float(int(b.numFlakes / treshold));

		D3DXMatrixIdentity(&World);
		if(numNeeded != 0)
		{
			color = 0x00FFFFFF | (int(toRender / treshold * 255.0f) << 24);
			D3DXMatrixTranslation(&World, -this->width + width / 2.0f + b.left, -this->height + height/2.0f + float(numNeeded) * moveUp  - start, 0.0f);
		}
		else
		{
			color = 0x00FFFFFF | (int(toRender / treshold * 255.0f) << 24);
			float h = height - toRender / treshold * height;
			D3DXMatrixTranslation(&World, -this->width + width / 2.0f + b.left, -this->height + height/2.0f - h - start, 0.0f);
		}
		v[0].color = v[1].color = v[2].color = v[3].color = color;
		
		
		device->SetTransform(D3DTS_WORLD, &World);
		device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, (UINT)2, (void*)&v[0], sizeof(TRANSLITVERTEX));
	}
};
