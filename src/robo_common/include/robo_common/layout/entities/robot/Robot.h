#ifndef ROBO_COMMON_ROBOT_H_
#define ROBO_COMMON_ROBOT_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "robo_common/defines/RoboCommonFunctionalDefines.h"
#include "robo_common/helpers/CollisionObject.h"
#include "robo_common/layout/entities/robot/animation/RobotAnimator.h"

//Forward declarations
class CollisionWatcher;
class RobotInitHelper;

struct RobotOutInterface {
  PlayerDamageCb playerDamageCb;
  SetFieldDataMarkerCb setFieldDataMarkerCb;
  ResetFieldDataMarkerCb resetFieldDataMarkerCb;
  GetFieldDescriptionCb getFieldDescriptionCb;
  FinishRobotActCb finishRobotActCb;
  CollisionWatcher *collisionWatcher = nullptr;
};

class Robot final : public CollisionObject {
public:
  friend class RobotInitHelper;

  int32_t init(const RobotState &initialState,
               const RobotAnimatorConfigBase &robotAnimCfgBase,
               const RobotOutInterface &interface,
               char fieldMarker);

  void deinit();
  void draw() const;

  RobotState getState() const;
  SurroundingTiles getSurroundingTiles() const;

  void act(MoveType moveType);

  void onMoveAnimEnd(Direction futureDir, const FieldPos &futurePos);
  void onCollisionImpactAnimEnd(RobotEndTurn status);
  void onCollisionImpact();

private:
  void onInitEnd();

  void registerCollision(const Rectangle &intersectRect,
                         CollisionDamageImpact impact) override;
  Rectangle getBoundary() const override;

  void move();

  RobotState _state;
  RobotOutInterface _outInterface;
  RobotAnimator _animator;
  CollisionWatchStatus _currCollisionWatchStatus = CollisionWatchStatus::OFF;
  char _fieldMarker = '?';
};

#endif /* ROBO_COMMON_ROBOT_H_ */
