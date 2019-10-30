// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include <Eigen/Core>
#include <memory>
#include <vector>

#include "Open3D/Geometry/Geometry3D.h"

namespace open3d {
namespace geometry {

class PointCloud;
class OrientedBoundingBox;
class AxisAlignedBoundingBox;
class TriangleMesh;
class TetraMesh;

/// \class LineSet
///
/// \brief LineSet define a sets of lines in 3D. A typical application is to
/// display the point cloud correspondence pairs.
class LineSet : public Geometry3D {
public:
    LineSet() : Geometry3D(Geometry::GeometryType::LineSet) {}
    LineSet(const std::vector<Eigen::Vector3d> &points,
            const std::vector<Eigen::Vector2i> &lines)
        : Geometry3D(Geometry::GeometryType::LineSet),
          points_(points),
          lines_(lines) {}
    ~LineSet() override {}

public:
    /// Clear all elements in the geometry.
    LineSet &Clear() override;
    /// Returns `true` iff the geometry is empty.
    bool IsEmpty() const override;
    /// Returns min bounds for geometry coordinates.
    Eigen::Vector3d GetMinBound() const override;
    /// Returns min bounds for geometry coordinates.
    Eigen::Vector3d GetMaxBound() const override;
    /// Returns the center of the geometry coordinates.
    Eigen::Vector3d GetCenter() const override;
    /// Returns an axis-aligned bounding box of the geometry.
    AxisAlignedBoundingBox GetAxisAlignedBoundingBox() const override;
    /// Returns an oriented bounding box of the geometry.
    OrientedBoundingBox GetOrientedBoundingBox() const override;
    /// Apply transformation (4x4 matrix) to the geometry coordinates.
    LineSet &Transform(const Eigen::Matrix4d &transformation) override;
    /// Apply translation to the geometry coordinates.
    ///
    /// \param translation - A 3D vector to transform the geometry.
    /// \param relative - If `true`, the translation vector is directly added to
    /// the geometry coordinates. Otherwise, the center is moved to the
    /// translation vector.
    LineSet &Translate(const Eigen::Vector3d &translation,
                       bool relative = true) override;
    /// Apply scaling to the geometry coordinates.
    ///
    /// \param scale -  The scale parameter that is multiplied to the
    /// points/vertices of the geometry.
    /// \param center - If `true`, then the scale is applied to the centered
    /// geometry.
    LineSet &Scale(const double scale, bool center = true) override;
    /// Apply rotation to the geometry coordinates and normals.
    ///
    /// \param R - A 3D vector that either defines the three angles for Euler
    /// rotation, or in the axis-angle representation the normalized vector
    /// defines the axis of rotation and the norm the angle around this axis.
    /// \param center - If `true`, then the rotation is applied to the centered
    /// geometry.
    LineSet &Rotate(const Eigen::Matrix3d &R, bool center = true) override;

    LineSet &operator+=(const LineSet &lineset);
    LineSet operator+(const LineSet &lineset) const;

    /// Returns `true` iff the number of points are greater than 0.
    bool HasPoints() const { return points_.size() > 0; }

    /// Returns `true` iff the number of lines are greater than 0 & HasPoints()
    /// is `true`.
    bool HasLines() const { return HasPoints() && lines_.size() > 0; }

    /// Returns `true` iff all lines have colors and HasLines() is `true`.
    bool HasColors() const {
        return HasLines() && colors_.size() == lines_.size();
    }

    /// \fn GetLineCoordinate
    ///
    /// \brief Returns the coordinates of the line at the given index.
    /// \param line_index - Index of the line.
    std::pair<Eigen::Vector3d, Eigen::Vector3d> GetLineCoordinate(
            size_t line_index) const {
        return std::make_pair(points_[lines_[line_index][0]],
                              points_[lines_[line_index][1]]);
    }

    /// \fn PaintUniformColor
    ///
    /// \brief Assigns each line in the LineSet the same color.
    /// \param color - specifies the color to be applied.
    LineSet &PaintUniformColor(const Eigen::Vector3d &color) {
        ResizeAndPaintUniformColor(colors_, lines_.size(), color);
        return *this;
    }

    /// \fn CreateFromPointCloudCorrespondences
    ///
    /// \brief Factory function to create a LineSet from two PointClouds
    /// (\param cloud0, \param cloud1) and a correspondence set.
    /// \param cloud0 - First point cloud.
    /// \param cloud1 - Second point cloud.
    /// \param correspondences - Set of correspondences..
    static std::shared_ptr<LineSet> CreateFromPointCloudCorrespondences(
            const PointCloud &cloud0,
            const PointCloud &cloud1,
            const std::vector<std::pair<int, int>> &correspondences);

    /// \fn CreateFromOrientedBoundingBox
    ///
    /// \brief Factory function to create a LineSet from an OrientedBoundingBox.
    /// \param box - The input bounding box.
    static std::shared_ptr<LineSet> CreateFromOrientedBoundingBox(
            const OrientedBoundingBox &box);

    /// \fn CreateFromAxisAlignedBoundingBox
    ///
    /// \brief Factory function to create a LineSet from an
    /// AxisAlignedBoundingBox. \param box - The input bounding box.
    static std::shared_ptr<LineSet> CreateFromAxisAlignedBoundingBox(
            const AxisAlignedBoundingBox &box);

    /// Factory function to create a LineSet from edges of a triangle mesh
    /// \param mesh.
    static std::shared_ptr<LineSet> CreateFromTriangleMesh(
            const TriangleMesh &mesh);

    /// Factory function to create a LineSet from edges of a tetra mesh
    /// \param mesh.
    static std::shared_ptr<LineSet> CreateFromTetraMesh(const TetraMesh &mesh);

public:
    /// Points coordinates.
    std::vector<Eigen::Vector3d> points_;
    /// Lines denoted by the index of points forming the line.
    std::vector<Eigen::Vector2i> lines_;
    /// RGB colors of lines.
    std::vector<Eigen::Vector3d> colors_;
};

}  // namespace geometry
}  // namespace open3d
