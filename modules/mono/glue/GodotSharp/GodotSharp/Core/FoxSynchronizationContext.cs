using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

namespace Fox
{
    public class FoxSynchronizationContext : SynchronizationContext
    {
        private readonly BlockingCollection<KeyValuePair<SendOrPostCallback, object>> _queue = new BlockingCollection<KeyValuePair<SendOrPostCallback, object>>();

        public override void Post(SendOrPostCallback d, object state)
        {
            _queue.Add(new KeyValuePair<SendOrPostCallback, object>(d, state));
        }

        public void ExecutePendingContinuations()
        {
            while (_queue.TryTake(out var workItem))
            {
                workItem.Key(workItem.Value);
            }
        }
    }
}
