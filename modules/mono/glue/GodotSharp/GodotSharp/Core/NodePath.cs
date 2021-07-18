using System;
using System.Runtime.CompilerServices;

namespace Fox
{
    public sealed partial class NodePath : IDisposable
    {
        private bool disposed = false;

        private IntPtr ptr;

        internal static IntPtr GetPtr(NodePath instance)
        {
            if (instance == null)
                throw new NullReferenceException($"The instance of type {nameof(NodePath)} is null.");

            if (instance.disposed)
                throw new ObjectDisposedException(instance.GetType().FullName);

            return instance.ptr;
        }

        ~NodePath()
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
            if (disposed)
                return;

            if (ptr != IntPtr.Zero)
            {
                Fox_icall_NodePath_Dtor(ptr);
                ptr = IntPtr.Zero;
            }

            disposed = true;
        }

        internal NodePath(IntPtr ptr)
        {
            this.ptr = ptr;
        }

        public NodePath() : this(string.Empty) {}

        public NodePath(string path)
        {
            ptr = Fox_icall_NodePath_Ctor(path);
        }

        public static implicit operator NodePath(string from) => new NodePath(from);

        public static implicit operator string(NodePath from) => from.ToString();

        public override string ToString()
        {
            return Fox_icall_NodePath_operator_String(GetPtr(this));
        }

        public NodePath GetAsPropertyPath()
        {
            return new NodePath(Fox_icall_NodePath_get_as_property_path(GetPtr(this)));
        }

        public string GetConcatenatedSubnames()
        {
            return Fox_icall_NodePath_get_concatenated_subnames(GetPtr(this));
        }

        public string GetName(int idx)
        {
            return Fox_icall_NodePath_get_name(GetPtr(this), idx);
        }

        public int GetNameCount()
        {
            return Fox_icall_NodePath_get_name_count(GetPtr(this));
        }

        public string GetSubname(int idx)
        {
            return Fox_icall_NodePath_get_subname(GetPtr(this), idx);
        }

        public int GetSubnameCount()
        {
            return Fox_icall_NodePath_get_subname_count(GetPtr(this));
        }

        public bool IsAbsolute()
        {
            return Fox_icall_NodePath_is_absolute(GetPtr(this));
        }

        public bool IsEmpty()
        {
            return Fox_icall_NodePath_is_empty(GetPtr(this));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr Fox_icall_NodePath_Ctor(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Fox_icall_NodePath_Dtor(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Fox_icall_NodePath_operator_String(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr Fox_icall_NodePath_get_as_property_path(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Fox_icall_NodePath_get_concatenated_subnames(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Fox_icall_NodePath_get_name(IntPtr ptr, int arg1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int Fox_icall_NodePath_get_name_count(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Fox_icall_NodePath_get_subname(IntPtr ptr, int arg1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int Fox_icall_NodePath_get_subname_count(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Fox_icall_NodePath_is_absolute(IntPtr ptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Fox_icall_NodePath_is_empty(IntPtr ptr);
    }
}
