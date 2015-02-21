// DesktopEffects SDK
// C# port revision 1
// ported by Ryan Petrich (rpetrich@gmail.com)

using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Reflection;
using Microsoft.Win32;

namespace AveDesktopEffects
{
    [Flags]
    public enum Features
    {
        None = 0,
        Configuration = 1,
        D3DDrawOverRealWallpaper = 2
    }

    public enum Notifications
    {
        BeforeWallpaperChanged=1,
        AfterWallpaperChanged=2
    }

    public enum GdiBackgroundUpdateActions
    {
        DoNotOverride = 0,
        DoNotClear = 1,
        DoClear = 2,
        WantsToRenderOverDefaultBG = 3
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct RECT
    {
        public int Left;
        public int Top;
        public int Right;
        public int Bottom;
    }

    [StructLayout(LayoutKind.Sequential, Pack=4)]
    public struct RemotableHandle
    {
        public int fContext;
        public IntPtr Value;
    }

    [ComImport, Guid("DC830845-6C17-48F9-932A-82979DFB3936"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IAveDesktopEffectsHost
    {
        void Start();
        void Stop();
        void IsRunning(out bool IsRunning);
        //void GetTargetWindow([Out, ComAliasName("AveDesktopEffectsLib.wireHWND")] IntPtr targetWindow);
        IntPtr GetTargetWindow();
        void LoadPluginByCLSID([In, MarshalAs(UnmanagedType.BStr)] string clsid);
        void IsD3DActive(out bool pIsActive);
        void D3DRenderIconBuffer([In] uint mode);
        void StartD3DMode();
        void StopD3DMode();
        void D3DRender();
        void MakeSureIsInitialized();
        void GetWallpaperTexture([MarshalAs(UnmanagedType.IUnknown)] out object texture);
        void SetWallpaperTexture([In, MarshalAs(UnmanagedType.IUnknown)] object texture);
        IntPtr GetHelperWindow([In, MarshalAs(UnmanagedType.IUnknown)] object effectAsUnknown);
    }

    [ComImport, Guid("77766B10-7FB8-49D6-895E-2A5B52EAB8E4"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IAveDesktopEffect
    {
        void GetName([MarshalAs(UnmanagedType.BStr)] out string pName);
        void GetAuthor([MarshalAs(UnmanagedType.BStr)] out string pAuthor);
        void GetDescription([MarshalAs(UnmanagedType.BStr)] out string pDescription);
        //void GetIcon([Out, ComAliasName("AveDesktopEffectsLib.wireHBITMAP")] IntPtr pHBitmap);
        void GetIcon(out IntPtr pHBitmap);
        void GetCooperationFlags(out uint pFlags);
        void OnInitialize([MarshalAs(UnmanagedType.Interface)] IAveDesktopEffectsHost host);
        void OnUninitialize();
        void OnStart();
        void OnStop();
        void OnPause();
        void OnResume();
        //void OnD3DStart([MarshalAs(UnmanagedType.Interface)] object directDevice);
        void OnD3DStart(IntPtr directDevice);
        void OnD3DStop();
        void OnBeforeGdiBackgroundUpdate(IntPtr hdc, [In] ref RECT updateRect, [In, Out] ref GdiBackgroundUpdateActions pWhatToDo);
        void OnAfterGdiBackgroundUpdate(IntPtr hdc, [In] ref RECT updateRect);
        void OnBeforeIconBufferUpdate();
        void OnAfterIconBufferUpdate();
        void OnD3DRender(out bool pHasRendered);
        void OnBeforeD3DRender(out uint pBgColor);
        void OnBeforeD3DSceneStart();
        void OnD3DSceneStart();
        void OnBeforeIconBufferRender();
        void OnAfterIconBufferRender();
        void OnD3DSceneEnd();
        void OnBeforeD3DPresent();
        void OnAfterD3DPresent();
        void OnAfterD3DRender();
        void OnTargetWindowMessage(IntPtr hwnd, uint msg, uint wParam, int lParam, out int lResult, out int bHandled);
        void GetConfigurationWindow(out IntPtr hwnd, IntPtr parent);
        void OnNotification(uint dwNotification);
        void DoesSupport(out uint pFlag);
        void OnNotificationWindowMessage(IntPtr hwnd, uint msg, uint wParam, int lParam, out int lResult, out int bHandled);
    }

    public sealed class AveDesktopEffectsHost : IAveDesktopEffectsHost
    {
        IAveDesktopEffectsHost host;

        public AveDesktopEffectsHost(IAveDesktopEffectsHost realHost)
        {
            if (realHost == null)
                throw new ArgumentNullException();
            host = realHost;
        }

        /// <summary>
        /// Makes DesktopEffects take over the desktop rendering from the shell so that an Effect can render onto the desktop.
        /// </summary>
        public void Start()
        {
            host.Start();
        }

        /// <summary>
        /// Makes DesktopEffects stop taking over the desktop rendering and let the shell render the desktop again.
        /// </summary>
        public void Stop()
        {
            host.Stop();
        }

        /// <summary>
        /// Returns true iff DesktopEffects is rendering the desktop.
        /// </summary>
        public bool IsRunning
        {
            get
            {
                bool isRunning = false;
                host.IsRunning(out isRunning);
                return isRunning;
            }
        }
        void IAveDesktopEffectsHost.IsRunning(out bool IsRunning)
        {
            host.IsRunning(out IsRunning);
        }

        /// <summary>
        /// Retrieves the window handle of the window where the desktop will be rendered onto.
        /// </summary>
        /// <returns>A window handle.</returns>
        public IntPtr GetTargetWindow()
        {
            return host.GetTargetWindow();
        }

        /// <summary>
        /// Loads an IAveDesktopEffect plugin by its CLSID.
        /// </summary>
        /// <param name="clsid">The ClassId of the plugin to load. The class must already be registered.</param>
        public void LoadPluginByCLSID(string clsid)
        {
            host.LoadPluginByCLSID(clsid);
        }

        /// <summary>
        /// Returns true iff is Direct3D rendering of the desktop is activated. If GDI rendering is enabled or the shell is still rendering the desktop, returns false.
        /// </summary>
        public bool IsD3DActive
        {
            get
            {
                bool isActive;
                host.IsD3DActive(out isActive);
                return isActive;
            }
        }
        void IAveDesktopEffectsHost.IsD3DActive(out bool pIsActive)
        {
            host.IsD3DActive(out pIsActive);
        }

        /// <summary>
        /// In D3D mode, DesktopEffects takes care of rendering everything other than the desktop background. Thus, The host will need to manage the icons on the desktop. Since some effects might want to peform special effects to the icons on the desktop, it is possible to let the host render the "icon buffer" into the current D3D scene by calling this method.
        /// </summary>
        /// <param name="mode">The method to draw the icon buffer.</param>
        public void D3DRenderIconBuffer(uint mode)
        {
            host.D3DRenderIconBuffer(mode);
        }
        /// <summary>
        /// Makes DesktopEffects render the desktop using Direct3D, instead of GDI. This call must be matched with a call to StopD3DMode().
        /// </summary>
        /// <remarks>
        /// Multiple calls to this function must be matched with the same number of calls to StopD3DMode() to stop rendering the desktop using Direct3D. In other words, calls to this function are counted.
        /// </remarks>
        public void StartD3DMode()
        {
            host.StartD3DMode();
        }

        /// <summary>
        /// Makes DesktopEffects stop rendering the desktop using Direct3D and return to GDI rendering. This call must be proceeded by a call to StartD3DMode.
        /// </summary>
        public void StopD3DMode()
        {
            host.StopD3DMode();
        }

        /// <summary>
        /// When in D3DMode, will make DesktopEffects render the desktop onto a D3D scene, while making callbacks to the current loaded IAveDesktopEffect. Basically, this function makes DesktopEffects do one cycle of the rendering loop directly.
        /// </summary>
        /// <remarks>
        /// It is to note that the desktop might still be rendered into a D3D scene without calling this function, because of updates to the desktop itself (such as selecting icons). If that is the case, DesktopEffects will act like D3DRender() was called and thus perform a render cycle when it thinks it is necessary.
        /// </remarks>
        public void D3DRender()
        {
            host.D3DRender();
        }

        /// <summary>
        /// Makes sure DesktopEffect is initialized properly. Normally, this function does not have to be called.
        /// </summary>
        public void MakeSureIsInitialized()
        {
            host.MakeSureIsInitialized();
        }

        /// <summary>
        /// In D3D rendering mode, this function will return a texture that reflects the user's bitmap wallpaper for use by the IAveDesktopEffect object.
        /// For example, an effect can use this function to get a texture of the user's wallpaper and render it completely morphed inside the D3D scene onto the desktop.
        /// </summary>
        public object WallpaperTexture
        {
            get
            {
                object texture;
                host.GetWallpaperTexture(out texture);
                return texture;
            }
            set
            {
                host.SetWallpaperTexture(value);
            }
        }
        void IAveDesktopEffectsHost.SetWallpaperTexture(object texture)
        {
            host.SetWallpaperTexture(texture);
        }
        void IAveDesktopEffectsHost.GetWallpaperTexture(out object texture)
        {
            host.GetWallpaperTexture(out texture);
        }

        /// <summary>
        /// This function will return a handle to a window that can be freely used by the calling effect for timers, thread marshalling and such. This "helper" window is created on demand and only once per effect and serves only as a convenience for effects so that they don't have to setup a helper window themselves.
        /// </summary>
        /// <param name="effect">Should pass reference to self</param>
        /// <returns>A window handle.</returns>
        public IntPtr GetHelperWindow(IAveDesktopEffect effect)
        {
            return host.GetHelperWindow(effect);
        }
        IntPtr IAveDesktopEffectsHost.GetHelperWindow(object effectAsUnknown)
        {
            return host.GetHelperWindow(effectAsUnknown);
        }

    }

    public abstract class AveDesktopEffect : IAveDesktopEffect
    {
        static Type[] types = Assembly.GetExecutingAssembly().GetTypes();

        #region Registration Functions
        [ComRegisterFunction]
        private static void OnRegister(string key)
        {
            string guid = GetGuid(key);
            if (string.IsNullOrEmpty(guid))
                return;
            string name = GetName(guid);
            if (string.IsNullOrEmpty(name))
                return;
            RegistryKey AveKey = Registry.LocalMachine.CreateSubKey(@"SOFTWARE\AveSoftware\AveDesktopEffects\Effects\" + name);
            AveKey.SetValue("classid", "{" + guid + "}", RegistryValueKind.String);
        }

        [ComUnregisterFunction]
        private static void OnUnregister(string key)
        {
            string guid = GetGuid(key);
            if (string.IsNullOrEmpty(guid))
                return;
            string name = GetName(guid);
            if (string.IsNullOrEmpty(name))
                return;
            Registry.LocalMachine.DeleteSubKey(@"SOFTWARE\AveSoftware\AveDesktopEffects\Effects\" + name);
        }
        private static string GetGuid(string key)
        {
            return key.Substring(key.IndexOf('{')+1, 36);
        }
        private static string GetName(string guid)
        {
            foreach (Type type in types) {
                object[] attribs = type.GetCustomAttributes(typeof(GuidAttribute), false);
                foreach (GuidAttribute attribute in attribs) {
                    if (attribute.Value == guid) {
                        AveDesktopEffect effect = (AveDesktopEffect)type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                        return effect.Name;
                    }
                }
            }
            return string.Empty;
        }
        #endregion

        string name;
        /// <summary>
        /// Gets called when DesktopEffects wants to know the name of the Effect. The callee should return the name of the effect.
        /// </summary>
        public string Name
        {
            get { return name; }
            protected set { name = value; }
        }
        void IAveDesktopEffect.GetName(out string pName)
        {
            pName = name;
        }

        string author;
        /// <summary>
        /// Gets called when DesktopEffects wants to know the author of the Effect. The callee should return the author of the effect.
        /// </summary>
        public string Author
        {
            get { return author; }
            protected set { author = value; }
        }
        void IAveDesktopEffect.GetAuthor(out string pAuthor)
        {
            pAuthor = author;
        }

        string description;
        /// <summary>
        /// Gets called when DesktopEffects wants to know the description of the Effect. The callee should return the description of the effect.
        /// </summary>
        public string Description
        {
            get { return description; }
            protected set { description = value; }
        }
        void IAveDesktopEffect.GetDescription(out string pDescription)
        {
            pDescription = description;
        }

        Bitmap icon;
        /// <summary>
        /// Gets called when DesktopEffects needs to display an icon for the effect. The callee should assign a 64x64 ARGB HBITMAP containing the icon for the effect.
        /// </summary>
        public Bitmap Icon
        {
            get { return icon; }
            protected set { icon = value; }
        }
        void IAveDesktopEffect.GetIcon(out IntPtr pHBitmap)
        {
            if (icon != null)
                pHBitmap = icon.GetHbitmap();
            else
                pHBitmap = IntPtr.Zero;
        }

        /// <summary>
        /// Not Implemented.
        /// </summary>
        void IAveDesktopEffect.GetCooperationFlags(out uint pFlags)
        {
            pFlags = 0;
        }

        /// <summary>
        /// Called when an Effect is loaded and will be used inside DesktopEffects.
        /// </summary>
        /// <remarks>
        /// Important: The effect should not start rendering as a result of this call nor ask the host to Start(), since it might be possible that an effect is initialized already, but the user does not want DesktopEffect to render.
        /// </remarks>
        public virtual void OnInitialize()
        {
        }
        void IAveDesktopEffect.OnInitialize(IAveDesktopEffectsHost host)
        {
            this.host = new AveDesktopEffectsHost(host);
            OnInitialize();
        }

        /// <summary>
        /// Called when an Effect is about to be unloaded. The Effect should release all resources it has.
        /// </summary>
        void IAveDesktopEffect.OnUninitialize()
        {
            OnUninitialize();
            this.host = null;
        }
        public virtual void OnUninitialize()
        {
        }

        /// <summary>
        /// Called when DesktopEffects took over the desktop rendering. At this point, the effect can start rendering in GDI mode or request to start rendering in D3D mode by calling the host's StartD3DMode() method.
        /// </summary>
        public virtual void OnStart()
        {
        }

        /// <summary>
        /// Called when DesktopEffects stops rendering the desktop or when the effect is unloaded. The effect should stop rendering in GDI mode or request to stop D3D mode rendering by calling the host's StopD3DMode() method.
        /// </summary>
        public virtual void OnStop()
        {
        }

        /// <summary>
        /// Called when rendering is paused and not performed by DesktopEffects.
        /// </summary>
        public virtual void OnPause()
        {
        }

        /// <summary>
        /// Called when rendering is resumed by DesktopEffects after a matching call to OnPause().
        /// </summary>
        public virtual void OnResume()
        {
        }

        IntPtr directDevice;
        /// <summary>
        /// Returns an IDirect3DDevice9 object used for rendering the desktop.
        /// </summary>
        public IntPtr Direct3DDevice
        {
            get { return directDevice; }
        }

        /// <summary>
        /// Called as a result of DesktopEffect starting up Direct3D rendering mode. At this point, the desktop will be rendered onto a D3D scene.
        /// </summary>
        public virtual void OnD3DStart()
        {
        }
        void IAveDesktopEffect.OnD3DStart(IntPtr directDevice)
        {
            this.directDevice = directDevice;
            Marshal.AddRef(directDevice);
            OnD3DStart();
        }

        /// <summary>
        /// Called whenever D3D mode rendering is stopped, either of a result of a call to the host's StopD3dMode() method, or of an external influence.
        /// The Effect should stop all D3D rendering and free all D3D rendering related resources.
        /// </summary>
        public virtual void OnD3DStop()
        {
        }
        void IAveDesktopEffect.OnD3DStop()
        {
            Marshal.Release(directDevice);
            directDevice = IntPtr.Zero;
            OnD3DStop();
        }

        #region GDI Render Cycle
        /// <summary>
        /// Called before the background of the desktop is updated in GDI rendering mode.
        /// </summary>
        /// <param name="hdc">A handle to the device context used for drawing and can be used to modify the background or its properties.</param>
        /// <param name="updateRect">A rectangle describing the part of the desktop that gets updated.</param>
        /// <param name="pWhatToDo"></param>
        public virtual void OnBeforeGdiBackgroundUpdate(IntPtr hdc, ref RECT updateRect, ref GdiBackgroundUpdateActions pWhatToDo)
        {
        }

        /// <summary>
        /// Called when the *pWhatToDo parameter of OnBeforeGdiBackgroundUpdate was set to AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG and the normal background (wallpaper) has been rendered into the hdc. Allows the Effect to paint over the normal background of the desktop.
        /// </summary>
        /// <param name="hdc">A handle to the device context used for drawing and can be used to modify the background or its properties.</param>
        /// <param name="updateRect">A rectangle describing the part of the desktop that gets updated.</param>
        public virtual void OnAfterGdiBackgroundUpdate(IntPtr hdc, ref RECT updateRect)
        {
        }
        #endregion

        #region Direct3D Render Cycle
        /// <summary>
        /// Called before the internal icon buffer is updated when in D3D rendering mode. The icon buffer is updated with the contents of the desktop icons whenever there are changes to the desktop icons and the next render cycle is started.
        /// </summary>
        /// <remarks>
        /// At this point, the icon buffer does not contain an up to date reflection of the desktop icons yet.
        /// </remarks>
        public virtual void OnBeforeIconBufferUpdate()
        {
        }

        /// <summary>
        /// Called after the internal icon buffer is updated when in D3D rendering mode. The icon buffer is updated with the contents of the desktop icons whenever there are changes to the desktop icons and the next render cycle is started. This callback might come into use when making use of the D3DRenderIconBuffer() function of the Host.
        /// </summary>
        /// <remarks>
        /// At this point, the icon buffer does contain an up to date reflection of the desktops icons.
        /// </remarks>
        public virtual void OnAfterIconBufferUpdate()
        {
        }

        /// <summary>
        /// Called when a D3D render cycle is started. This is usually initiated by the Effect calling the Host's D3DRender() method or by an external update to the desktop icons.
        /// </summary>
        /// <remarks>
        /// This callback allows the Effect to take over the render cycle completely (beginning a scene, drawing into the scene, drawing the icon buffer, closing the scene and presenting). If an Effect chooses to do this, it must set *pHasRendered to TRUE so that DesktopEffects knows that the render cycle has been performed already.
        /// Normally, an Effect does nothing in this callback, but some Effects might want to take over the render cycle for performance reasons (for example, video rendering).
        /// </remarks>
        /// <param name="pHasRendered">Set to true if taking over render cycle.</param>
        public virtual void OnD3DRender(out bool pHasRendered)
        {
            pHasRendered = false;
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle will be started (thus, when it was not taken over by the effect in the OnD3DRender() callback).
        /// </summary>
        /// <remarks>
        /// An effect gets the chance to do per-cycle initialization here and, change the background color of the D3D scene by setting the pBgColor parameter to the preferred ARGB color.
        /// </remarks>
        /// <param name="pBgColor">The new background color of the D3D scene.</param>
        public virtual void OnBeforeD3DRender(out uint pBgColor)
        {
            pBgColor = 0xFFFFFFFF;
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle has been started after the device has been cleared, but before the D3D scene is started.
        /// </summary>
        /// <remarks>
        /// The Effect can modify D3D parameters at this point.
        /// </remarks>
        public virtual void OnBeforeD3DSceneStart()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle has started the D3D scene and did set it up correctly.
        /// By default, the Device is in a 2D render state with a quad VertexBuffer that is the size of the desktop, with direct mapping texture coords.
        /// </summary>
        /// <remarks>
        /// If the AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER flag was returned by this Effect, the user's wallpaper will be drawn into the D3D scene at this point.
        /// Usually, this is the function where a regular D3D effect does its own rendering of the desktops background
        /// </remarks>
        public virtual void OnD3DSceneStart()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle has called the Effects OnD3DSceneStart() callback and DesktopEffects is about to render the desktops icons onto the D3D scene.
        /// </summary>
        /// <remarks>
        /// At this point, the scene has the desktop background rendered, but not yet the icons.
        /// </remarks>
        public virtual void OnBeforeIconBufferRender()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle has called the Effects OnD3DSceneStart() callback and DesktopEffects has rendered the desktop icons into the D3D scene.
        /// </summary>
        /// <remarks>
        /// At this point, the scene has the desktop background rendered and the desktop icons.
        /// </remarks>
        public virtual void OnAfterIconBufferRender()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle has drawn everything in the D3D scene and will end the scene.
        /// </summary>
        /// <remarks>
        /// At this point, the scene has completely be rendered.
        /// </remarks>
        public virtual void OnD3DSceneEnd()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle is about to present the scene onto the real desktop.
        /// </summary>
        /// <remarks>
        /// At this point, the scene is completely rendered and ended.
        /// </remarks>
        public virtual void OnBeforeD3DPresent()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle presented the D3D scene onto the real desktop.
        /// </summary>
        /// <remarks>
        /// At this point, the user sees the just rendered frame on the real desktop.
        /// </remarks>
        public virtual void OnAfterD3DPresent()
        {
        }

        /// <summary>
        /// Called when DesktopEffects' own rendering cycle has been completed.
        /// </summary>
        /// <remarks>
        /// At this point, the D3D rendering cycle has been completed.
        /// </remarks>
        public virtual void OnAfterD3DRender()
        {
        }

        #endregion

        /// <summary>
        /// Called when a Window Message is received by the target window on which the render will be performed.
        /// The Effect get the possibility to, for example, track mouse movements on the desktop by tracing WM_MOUSEMOVE messages and responding accordingly. A good example would be a sparkle that follows the mouse cursor by watching for WM_MOUSEMOVE messages and adding sparks to the D3D scene on the places where the mouse is located.
        /// </summary>
        /// <param name="bHandled">Set to 1 if the message has been handled.</param>
        public virtual void OnTargetWindowMessage(IntPtr hwnd, uint msg, uint wParam, int lParam, out int lResult, out int bHandled)
        {
            lResult = 0;
            bHandled = 0;
        }

        /// <summary>
        /// Called when a configuration window for this Effect is needed and the effect returned the AVE_SUPPORTS_CONFIG flag.
        /// </summary>
        public virtual IntPtr GetConfigurationWindow(IntPtr parentHWnd)
        {
            return IntPtr.Zero;
        }
        void IAveDesktopEffect.GetConfigurationWindow(out IntPtr hwnd, IntPtr parent)
        {
            hwnd = GetConfigurationWindow(parent);
        }

        /// <summary>
        /// Called when the Host needs to notify the Effect.
        /// </summary>
        /// <param name="type">The type of event the effect is being informed of.</param>
        public virtual void OnNotification(Notifications type)
        {
        }
        void IAveDesktopEffect.OnNotification(uint dwNotification)
        {
            OnNotification((Notifications)dwNotification);
        }

        Features doesSupport;
        /// <summary>
        /// Called when DesktopEffects needs to know what the Effect supports.
        /// </summary>
        public Features SupportedFeatures
        {
            get { return doesSupport; }
            protected set { doesSupport = value; }
        }
        void IAveDesktopEffect.DoesSupport(out uint pFlag)
        {
            pFlag = (uint)doesSupport;
        }

        /// <summary>
        /// Called when a Window Message is received by the helper window of the effect.
        /// </summary>
        public virtual void OnNotificationWindowMessage(IntPtr hwnd, uint msg, uint wParam, int lParam, out int lResult, out int bHandled)
        {
            lResult = 0;
            bHandled = 0;
        }

        #region Host
        private AveDesktopEffectsHost host;
        public AveDesktopEffectsHost Host
        {
            get { return host; }
        }
        #endregion

    }

}