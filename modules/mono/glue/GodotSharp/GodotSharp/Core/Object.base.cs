using System;
using System.Runtime.CompilerServices;

namespace Fox
{
    public partial class Object : IDisposable
    {
        private bool disposed = false;

        private static StringName nativeName = "Object";

        internal IntPtr ptr;
        internal bool memoryOwn;

        public Object() : this(false)
        {
            if (ptr == IntPtr.Zero)
                ptr = Fox_icall_Object_Ctor(this);
            _InitializeFoxScriptInstanceInternals();
        }

        internal void _InitializeFoxScriptInstanceInternals()
        {
            Fox_icall_Object_ConnectEventSignals(ptr);
        }

        internal Object(bool memoryOwn)
        {
            this.memoryOwn = memoryOwn;
        }

        public IntPtr NativeInstance
        {
            get { return ptr; }
        }

        internal static IntPtr GetPtr(Object instance)
        {
            if (instance == null)
                return IntPtr.Zero;

            if (instance.disposed)
                throw new ObjectDisposedException(instance.GetType().FullName);

            return instance.ptr;
        }

        ~Object()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposed)
                return;

            if (ptr != IntPtr.Zero)
            {
                if (memoryOwn)
                {
                    memoryOwn = false;
                    Fox_icall_RefCounted_Disposed(this, ptr, !disposing);
                }
                else
                {
                    Fox_icall_Object_Disposed(this, ptr);
                }

                this.ptr = IntPtr.Zero;
            }

            disposed = true;
        }

        public override string ToString()
        {
            return Fox_icall_Object_ToString(GetPtr(this));
        }

        /// <summary>
        /// Returns a new <see cref="Fox.SignalAwaiter"/> awaiter configured to complete when the instance
        /// <paramref name="source"/> emits the signal specified by the <paramref name="signal"/> parameter.
        /// </summary>
        /// <param name="source">
        /// The instance the awaiter will be listening to.
        /// </param>
        /// <param name="signal">
        /// The signal the awaiter will be waiting for.
        /// </param>
        /// <example>
        /// This sample prints a message once every frame up to 100 times.
        /// <code>
        /// public override void _Ready()
        /// {
        ///     for (int i = 0; i &lt; 100; i++)
        ///     {
        ///         await ToSignal(GetTree(), "idle_frame");
        ///         GD.Print($"Frame {i}");
        ///     }
        /// }
        /// </code>
        /// </example>
        public SignalAwaiter ToSignal(Object source, StringName signal)
        {
            return new SignalAwaiter(source, signal, this);
        }

        /// <summary>
        /// Gets a new <see cref="Fox.DynamicFoxObject"/> associated with this instance.
        /// </summary>
        public dynamic DynamicObject => new DynamicFoxObject(this);

        internal static IntPtr __ClassDB_get_method(StringName type, string method)
        {
            return Fox_icall_Object_ClassDB_get_method(StringName.GetPtr(type), method);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr Fox_icall_Object_Ctor(Object obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Fox_icall_Object_Disposed(Object obj, IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Fox_icall_RefCounted_Disposed(Object obj, IntPtr ptr, bool isFinalizer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Fox_icall_Object_ConnectEventSignals(IntPtr obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string Fox_icall_Object_ToString(IntPtr ptr);

        // Used by the generated API
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr Fox_icall_Object_ClassDB_get_method(IntPtr type, string method);
    }
}
