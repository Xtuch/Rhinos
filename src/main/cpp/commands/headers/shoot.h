#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/ShooterSubsystem.h"

class shoot : public frc2::CommandHelper<frc2::Command, shoot> {
 public:
  explicit shoot(ShooterSubsystem* subsystem);

  void Initialize() override;

  bool IsFinished() override;

 private:
  ShooterSubsystem* m_hatch;
};