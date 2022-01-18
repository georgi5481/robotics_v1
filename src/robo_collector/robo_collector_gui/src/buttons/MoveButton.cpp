//Corresponding header
#include "robo_collector_gui/buttons/MoveButton.h"

//C system headers

//C++ system headers

//Other libraries headers
#include "sdl_utils/input/InputEvent.h"
#include "utils/ErrorCode.h"
#include "utils/Log.h"

//Own components headers

int32_t MoveButton::init(const MoveButtonCfg& cfg) {
  if (nullptr == cfg.moveCb) {
    LOGERR("Error, null moveCb detected for MoveButton with rsrcId: %#16lX",
        cfg.rsrcId);
    return FAILURE;
  }
  _moveCb = cfg.moveCb;

  if (MoveType::UNKNOWN == cfg.moveType) {
    LOGERR("MoveType::UNKNOWN detected for MoveButton with rsrcId: %#16lX",
        cfg.rsrcId);
    return FAILURE;
  }
  _moveType = cfg.moveType;

  create(cfg.rsrcId);
  setPosition(cfg.startPos);
  return SUCCESS;
}

void MoveButton::handleEvent(const InputEvent& e) {
  if (TouchEvent::TOUCH_PRESS == e.type) {
    setFrame(CLICKED);
  } else if (TouchEvent::TOUCH_RELEASE == e.type) {
    setFrame(UNCLICKED);
    _moveCb(_moveType);
  }
}

