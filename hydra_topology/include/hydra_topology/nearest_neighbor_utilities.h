#pragma once
#include "hydra_topology/voxblox_types.h"
#include "hydra_topology/dsg_types.h"

#include <memory>
#include <unordered_set>
#include <vector>

namespace hydra {
namespace topology {

// TODO(nathan) this probably belongs in kimera_dsg
class NearestNodeFinder {
 public:
  using Callback = std::function<void(NodeId, size_t, double)>;

  NearestNodeFinder(const SceneGraphLayer& layer, const std::vector<NodeId>& nodes);

  NearestNodeFinder(const SceneGraphLayer& layer,
                    const std::unordered_set<NodeId>& nodes);

  virtual ~NearestNodeFinder();

  void find(const Eigen::Vector3d& position,
            size_t num_to_find,
            bool skip_first,
            const Callback& callback);

 private:
  struct Detail;

  std::unique_ptr<Detail> internals_;
};

class NearestVoxelFinder {
 public:
  using Callback = std::function<void(const GlobalIndex&, size_t, int64_t)>;

  explicit NearestVoxelFinder(const voxblox::AlignedVector<GlobalIndex>& indices);

  virtual ~NearestVoxelFinder();

  void find(const GlobalIndex& index, size_t num_to_find, const Callback& callback);

 private:
  struct Detail;

  std::unique_ptr<Detail> internals_;
};

struct FurthestIndexResult {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  bool valid = false;
  int64_t distance = 0;
  bool from_source = true;
  GlobalIndex index;
};

FurthestIndexResult findFurthestIndexFromLine(
    const voxblox::AlignedVector<GlobalIndex>& indices,
    const GlobalIndex& start,
    const GlobalIndex& end,
    size_t number_source_edges);

inline FurthestIndexResult findFurthestIndexFromLine(
    const voxblox::AlignedVector<GlobalIndex>& indices,
    const GlobalIndex& start,
    const GlobalIndex& end) {
  return findFurthestIndexFromLine(indices, start, end, indices.size());
}

}  // namespace topology
}  // namespace hydra