//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_RUNTIME_H
#define LIBEVENT_TEST1_RUNTIME_H

#include "absl/container/flat_hash_map.h"
#include "absl/container/node_hash_map.h"
#include "absl/types/optional.h"

/**
 * Loads runtime snapshots from storage (local disk, etc.).
 */
class Loader {
public:
    virtual ~Loader() = default;

    using ReadyCallback = std::function<void()>;

    /**
     * Post-construction initialization. Runtime will be generally available after
     * the constructor is finished, with the exception of dynamic RTDS layers,
     * which require ClusterManager.
     * @param cm cluster manager reference.
     */
    //virtual void initialize(Upstream::ClusterManager& cm) = 0;

    /**
     * @return const Snapshot& the current snapshot. This reference is safe to use for the duration of
     *         the calling routine, but may be overwritten on a future event loop cycle so should be
     *         fetched again when needed. This may only be called from worker threads.
     */
    //virtual const Snapshot& snapshot() = 0;

    /**
     * @return shared_ptr<const Snapshot> the current snapshot. This function may safely be called
     *         from non-worker threads.
     */
    //virtual SnapshotConstSharedPtr threadsafeSnapshot() = 0;

    /**
     * Merge the given map of key-value pairs into the runtime's state. To remove a previous merge for
     * a key, use an empty string as the value.
     * @param values the values to merge
     */
    virtual void mergeValues(const absl::node_hash_map<std::string, std::string>& values) = 0;

    /**
     * Initiate all RTDS subscriptions. The `on_done` callback is invoked when all RTDS requests
     * have either received and applied their responses or timed out.
     */
    virtual void startRtdsSubscriptions(ReadyCallback on_done) = 0;

    /**
     * @return Stats::Scope& the root scope.
     */
    //virtual Stats::Scope& getRootScope() = 0;

    /**
     * Updates deprecated feature use stats.
     */
    virtual void countDeprecatedFeatureUse() const = 0;
};

using LoaderPtr = std::unique_ptr<Loader>;

// To make the runtime generally accessible, we make use of the dreaded
// singleton class. For Envoy, the runtime will be created and cleaned up by the
// Server::InstanceImpl initialize() and destructor, respectively.
//
// This makes it possible for call sites to easily make use of runtime values to
// determine if a given feature is on or off, as well as various deprecated configuration
// protos being enabled or disabled by default.
//using LoaderSingleton = InjectableSingleton<Loader>;
//using ScopedLoaderSingleton = ScopedInjectableLoader<Loader>;

#endif //LIBEVENT_TEST1_RUNTIME_H
