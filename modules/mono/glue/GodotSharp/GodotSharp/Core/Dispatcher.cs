using System.Runtime.CompilerServices;

namespace Fox
{
    public static class Dispatcher
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern FoxTaskScheduler Fox_icall_DefaultFoxTaskScheduler();

        public static FoxSynchronizationContext SynchronizationContext =>
            Fox_icall_DefaultFoxTaskScheduler().Context;
    }
}
