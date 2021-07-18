using System;
using System.Runtime.CompilerServices;

namespace Fox
{
    public sealed partial class StringName : IDisposable
    {
        private IntPtr ptr;

        internal static IntPtr GetPtr(StringName instance)
        {
            if (instance == null)
                throw new NullReferenceException($"The instance of type {nameof(StringName)} is null.");

            if (instance.ptr == IntPtr.Zero)
                throw new ObjectDisposedException(instance.GetType().FullName);

            return instance.ptr;
        }

        ~StringName()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (ptr != IntPtr.Zero)
            {
                Fox_icall_StringName_Dtor(ptr);
                ptr = IntPtr.Zero;
            }
        }

        internal StringName(IntPtr ptr)
        {
            this.ptr = ptr;
        }

        public StringName()
        {
            ptr = IntPtr.Zero;
        }

        public StringName(string path)
        {
            ptr = path == null ? IntPtr.Zero : Fox_icall_StringName_Ctor(path);
        }

        public static implicit operator StringName(string from) => new StringName(from);

        public static implicit operator string(StringName from) => from.ToString();

        public override string ToString()
        {
            return ptr == IntPtr.Zero ? string.Empty : Fox_icall_StringName_operator_String(GetPtr(this));
        }

        public bool IsEmpty()
        {
            return ptr == IntPtr.Zero || Fox_icall_StringName_is_empty(GetPtr(this));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr Fox_icall_StringName_Ctor(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Fox_icall_StringName_Dtor(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Fox_icall_StringName_operator_String(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Fox_icall_StringName_is_empty(IntPtr ptr);
    }
}
