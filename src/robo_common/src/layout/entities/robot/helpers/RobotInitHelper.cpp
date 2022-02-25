//Corresponding header
#include "robo_common/layout/entities/robot/helpers/RobotInitHelper.h"

//C system headers

//C++ system headers

//Other libraries headers
#include "utils/ErrorCode.h"
#include "utils/Log.h"

//Own components headers
#include "robo_common/layout/entities/robot/Robot.h"

int32_t RobotInitHelper::init(const RobotState &initialState,
                              const RobotAnimatorConfigBase &robotAnimCfgBase,
                              const RobotOutInterface &interface,
                              Robot &robot) {
  robot._state = initialState;

  if (SUCCESS != initOutInterface(interface, robot)) {
    LOGERR("Error, initOutInterface() failed");
    return FAILURE;
  }

  if (SUCCESS != initAnimator(robotAnimCfgBase, robot)) {
    LOGERR("Error, initAnimator() failed");
    return FAILURE;
  }

  return SUCCESS;
}

int32_t RobotInitHelper::initOutInterface(const RobotOutInterface &interface,
                                          Robot &robot) {
  robot._outInterface = interface;

  if (nullptr == robot._outInterface.playerDamageCb) {
    LOGERR("Error, nullptr provided for PlayerDamageCb");
    return FAILURE;
  }

  // only player robot should report damage callback
  if (RoboCommonDefines::PLAYER_ROBOT_IDX != robot._state.robotId) {
    robot._outInterface.playerDamageCb = nullptr;
  }

  if (nullptr == robot._outInterface.setFieldDataMarkerCb) {
    LOGERR("Error, nullptr provided for setFieldDataMarkerCb");
    return FAILURE;
  }

  if (nullptr == robot._outInterface.resetFieldDataMarkerCb) {
    LOGERR("Error, nullptr provided for resetFieldDataMarkerCb");
    return FAILURE;
  }

  if (nullptr == robot._outInterface.finishRobotActCb) {
    LOGERR("Error, nullptr provided for finishRobotActCb");
    return FAILURE;
  }

  if (nullptr == robot._outInterface.getFieldDescriptionCb) {
    LOGERR("Error, nullptr provided for GetFieldDescriptionCb");
    return FAILURE;
  }

  if (nullptr == robot._outInterface.collisionWatcher) {
    LOGERR("Error, nullptr provided for collisionWatcher");
    return FAILURE;
  }

  return SUCCESS;
}

int32_t RobotInitHelper::initAnimator(
    const RobotAnimatorConfigBase &robotAnimCfgBase, Robot &robot) {
  using namespace std::placeholders;

  RobotAnimatorConfig cfg;
  cfg.baseCfg = robotAnimCfgBase;
  cfg.startDir = robot._state.dir;
  cfg.startPos = robot._state.fieldPos;

  RobotAnimatorOutInterface interface;
  interface.onMoveAnimEndCb = std::bind(&Robot::onMoveAnimEnd, &robot, _1, _2);
  interface.collisionImpactAnimEndCb =
      std::bind(&Robot::onCollisionImpactAnimEnd, &robot, _1);
  interface.collisionImpactCb = std::bind(&Robot::onCollisionImpact, &robot);
  interface.getRobotStateCb = std::bind(&Robot::getState, &robot);
  interface.getFieldDescriptionCb = robot._outInterface.getFieldDescriptionCb;

  if (SUCCESS != robot._animator.init(cfg, interface)) {
    LOGERR("Error, RobotAnimator.init() failed");
    return FAILURE;
  }

  return SUCCESS;
}

