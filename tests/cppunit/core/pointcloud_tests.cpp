/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/datasource/pointcloud.h"
#include <algorithm>
#include <random>

using namespace earthxpert;

static constexpr auto rngMax = static_cast<double>(std::mt19937::max());
static constexpr auto rngMid = rngMax / 2;
static constexpr auto scaleX = 1000. / (rngMax + 2.0);
static constexpr auto scaleY = 1000. / (rngMax + 2.0);
static constexpr auto scaleZ = 100. / (rngMax + 2.0);

static void FillXYZ(std::vector<XYZF>& xyz, std::mt19937& rng) {
  std::generate_n(std::begin(xyz), xyz.size(), [&rng]() {
    return XYZF{ float((double(rng()) - rngMid) * scaleX),
                 float((double(rng()) - rngMid) * scaleY),
                 float((double(rng()) - rngMid) * scaleZ) };
    });
}

static void FillCla(std::vector<Classification>& cla, std::mt19937& rng) {
  std::generate_n(std::begin(cla), cla.size(), [&rng]() {
    return Classification{ rng() % 128 };
    });
}

static void CheckPointcloud_PointF(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    auto point = pc.GetPoint<PointF>(i);
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);
  }
}

static void CheckPointcloud_PointF_Subscript(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    auto point = static_cast<PointF>(pc[i]);
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);

    auto cvpoint = static_cast<Point3f>(point);
    ASSERT_GE(cvpoint.x, -500.0f);
    ASSERT_LE(cvpoint.x, 500.0f);
    ASSERT_GE(cvpoint.y, -500.0f);
    ASSERT_LE(cvpoint.y, 500.0f);
    ASSERT_GE(cvpoint.z, -50.0f);
    ASSERT_LE(cvpoint.z, 50.0f);
  }
}

static void CheckPointcloud_CPointF(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    auto point = pc.GetPoint<CPointF>(i);
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);
    ASSERT_LT(point.c, 128);
  }
}

static void CheckPointcloud_CPointF_Subscript(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    CPointF point = pc[i];
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);
    ASSERT_LT(point.c, 128);
  }
}

TEST(pointcloud, Constructor_PointCount_FieldGroup) {
  FieldGroup fieldGroup;
  fieldGroup.set(FieldGroup::kPOSXYZ);
  fieldGroup.set(FieldGroup::kPOSCLA);

  Pointcloud pc(100, fieldGroup);
  ASSERT_EQ(100, pc.GetPointCount());
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSXYZ));
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSCLA));
  ASSERT_FALSE(pc.GetFieldGroup().test(FieldGroup::kPOSINT));
  ASSERT_FALSE(pc.GetFieldGroup().test(FieldGroup::kPOSRGB));

  Point3d origin{ 1000.0, 2000.0, 0.0 };
  pc.SetOrigin(origin);
  ASSERT_EQ(origin, pc.GetOrigin());

  auto& xyz = pc.GetXYZ();
  auto& cla = pc.GetCla();

  std::mt19937 rng;
  FillXYZ(xyz, rng);
  FillCla(cla, rng);

  CheckPointcloud_CPointF(pc);
}

TEST(pointcloud, Constructor_PointF) {
  Point3d origin{ 1000.0, 2000.0, 0.0 };
  std::vector<XYZF> xyz(100);

  std::mt19937 rng;
  FillXYZ(xyz, rng);

  Pointcloud pc(origin, std::move(xyz));

  ASSERT_EQ(100, pc.GetPointCount());
  ASSERT_EQ(origin, pc.GetOrigin());
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSXYZ));

  CheckPointcloud_PointF_Subscript(pc);
}

TEST(pointcloud, Constructor_CPointF) {
  Point3d origin{ 1000.0, 2000.0, 0.0 };
  std::vector<XYZF> xyz(100);
  std::vector<Classification> cla(100);

  std::mt19937 rng;
  FillXYZ(xyz, rng);
  FillCla(cla, rng);

  Pointcloud pc(origin, std::move(xyz), std::move(cla));

  ASSERT_EQ(100, pc.GetPointCount());
  ASSERT_EQ(origin, pc.GetOrigin());
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSXYZ));
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSCLA));

  CheckPointcloud_CPointF(pc);
}

TEST(pointcloud, SetPoint) {
  FieldGroup fieldGroup;
  fieldGroup.set(FieldGroup::kPOSXYZ);
  fieldGroup.set(FieldGroup::kPOSCLA);

  Pointcloud pc(100, fieldGroup);
  ASSERT_EQ(100, pc.GetPointCount());
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSXYZ));
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSCLA));

  Point3d origin{ 1000.0, 2000.0, 0.0 };
  pc.SetOrigin(origin);
  ASSERT_EQ(origin, pc.GetOrigin());

  std::mt19937 rng;
  for (size_t i = 0; i < 100; ++i) {
    CPointF point{ { float((double(rng()) - rngMid) * scaleX),
                   float((double(rng()) - rngMid) * scaleY),
                   float((double(rng()) - rngMid) * scaleZ) },
                  rng() % 128 };
    pc.SetPoint<CPointF>(i, point);
  }

  CheckPointcloud_CPointF_Subscript(pc);
}

TEST(pointcloud, Subscript) {
  FieldGroup fieldGroup;
  fieldGroup.set(FieldGroup::kPOSXYZ);
  fieldGroup.set(FieldGroup::kPOSCLA);

  Pointcloud pc(100, fieldGroup);
  ASSERT_EQ(100, pc.GetPointCount());
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSXYZ));
  ASSERT_TRUE(pc.GetFieldGroup().test(FieldGroup::kPOSCLA));

  Point3d origin{ 1000.0, 2000.0, 0.0 };
  pc.SetOrigin(origin);
  ASSERT_EQ(origin, pc.GetOrigin());

  std::mt19937 rng;
  for (size_t i = 0; i < 100; ++i) {
    pc[i] = CPointF{ { float((double(rng()) - rngMid) * scaleX),
                       float((double(rng()) - rngMid) * scaleY),
                       float((double(rng()) - rngMid) * scaleZ) },
                       rng() % 128 };
  }

  CheckPointcloud_CPointF_Subscript(pc);

  auto bbox = pc.CalcBoundingBox2D();
  ASSERT_GE(bbox.minX, 500.0);
  ASSERT_GE(bbox.minY, 1500.0);
  ASSERT_LE(bbox.width, 1000.0);
  ASSERT_LE(bbox.height, 1000.0);
}
