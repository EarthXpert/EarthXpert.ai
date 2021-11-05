/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/datasource/pointcloud.h"
#include <algorithm>
#include <random>

using namespace earthxpert;

constexpr float kTol{ 0.000001f };
constexpr auto kRngMax = static_cast<double>(std::mt19937::max());
constexpr auto kRngMid = kRngMax / 2;
constexpr auto kScaleX = 1000. / (kRngMax + 2.0);
constexpr auto kScaleY = 1000. / (kRngMax + 2.0);
constexpr auto kScaleZ = 100. / (kRngMax + 2.0);

void FillXYZ(std::vector<XYZF>& xyz, std::mt19937& rng) {
  std::generate_n(std::begin(xyz), xyz.size(), [&rng]() {
    return XYZF{ float((double(rng()) - kRngMid) * kScaleX),
                 float((double(rng()) - kRngMid) * kScaleY),
                 float((double(rng()) - kRngMid) * kScaleZ) };
    });
}

void FillCla(std::vector<Classification>& cla, std::mt19937& rng) {
  std::generate_n(std::begin(cla), cla.size(), [&rng]() {
    return Classification{ rng() % 128 };
    });
}

void CheckPointcloud_PointF(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    auto point = pc.GetPoint<PointF>(i);
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);

    auto& xyz = pc.GetField<XYZF>(i);
    ASSERT_NEAR(point.x, xyz.x, kTol);
    ASSERT_NEAR(point.y, xyz.y, kTol);
    ASSERT_NEAR(point.z, xyz.z, kTol);
  }
}

void CheckPointcloud_PointF_Subscript(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    auto point = static_cast<PointF>(pc[i]);
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);

    auto cvpoint = static_cast<Point3f>(point);
    ASSERT_NEAR(point.x, cvpoint.x, kTol);
    ASSERT_NEAR(point.y, cvpoint.y, kTol);
    ASSERT_NEAR(point.z, cvpoint.z, kTol);

    auto& xyz = pc.GetField<XYZF>(i);
    ASSERT_NEAR(point.x, xyz.x, kTol);
    ASSERT_NEAR(point.y, xyz.y, kTol);
    ASSERT_NEAR(point.z, xyz.z, kTol);
  }
}

void CheckPointcloud_CPointF(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    auto point = pc.GetPoint<CPointF>(i);
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);
    ASSERT_LT(point.c, 128);

    auto& xyz = pc.GetField<XYZF>(i);
    ASSERT_NEAR(point.x, xyz.x, kTol);
    ASSERT_NEAR(point.y, xyz.y, kTol);
    ASSERT_NEAR(point.z, xyz.z, kTol);

    auto& c = pc.GetField<Classification>(i);
    ASSERT_EQ(point.c, c.c);
  }
}

void CheckPointcloud_CPointF_Subscript(const Pointcloud& pc) {
  for (size_t i = 0; i < pc.GetPointCount(); ++i) {
    CPointF point = pc[i];
    ASSERT_GE(point.x, -500.0f);
    ASSERT_LE(point.x, 500.0f);
    ASSERT_GE(point.y, -500.0f);
    ASSERT_LE(point.y, 500.0f);
    ASSERT_GE(point.z, -50.0f);
    ASSERT_LE(point.z, 50.0f);
    ASSERT_LT(point.c, 128);

    auto& xyz = pc.GetField<XYZF>(i);
    ASSERT_NEAR(point.x, xyz.x, kTol);
    ASSERT_NEAR(point.y, xyz.y, kTol);
    ASSERT_NEAR(point.z, xyz.z, kTol);

    auto& c = pc.GetField<Classification>(i);
    ASSERT_EQ(point.c, c.c);
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

  auto& xyz = pc.GetField<XYZF>();
  auto& cla = pc.GetField<Classification>();

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
    CPointF point{ { float((double(rng()) - kRngMid) * kScaleX),
                   float((double(rng()) - kRngMid) * kScaleY),
                   float((double(rng()) - kRngMid) * kScaleZ) },
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
    pc[i] = CPointF{ { float((double(rng()) - kRngMid) * kScaleX),
                       float((double(rng()) - kRngMid) * kScaleY),
                       float((double(rng()) - kRngMid) * kScaleZ) },
                       rng() % 128 };
  }

  CheckPointcloud_CPointF_Subscript(pc);

  auto bbox = pc.GetBufTile();
  ASSERT_GE(bbox.minX, 500.0);
  ASSERT_GE(bbox.minY, 1500.0);
  ASSERT_LE(bbox.width, 1000.0);
  ASSERT_LE(bbox.height, 1000.0);
}
