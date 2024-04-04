// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc/controller/PIDController.h>
#include <frc/geometry/Translation2d.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/trajectory/Trajectory.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/SwerveControllerCommand.h>
#include <frc2/command/button/JoystickButton.h>
#include <units/angle.h>
#include <units/velocity.h>

#include <utility>

#include "Constants.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/IntakeSubsystem.h"
#include "subsystems/ArmSubsystem.h"
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include <frc/controller/SimpleMotorFeedforward.h>

using namespace DriveConstants;
using namespace ArmConstants;

RobotContainer::RobotContainer() {
  // Configure the button bindings
  ConfigureButtonBindings();

  // Set up default drive command
  // The left stick controls translation of the robot.
  // Turning is controlled by the X axis of the right stick.
  m_drive.SetDefaultCommand(frc2::RunCommand(
      [this] {
        m_drive.Drive(
            -units::meters_per_second_t{frc::ApplyDeadband(m_driverController.GetRawAxis(OIConstants::kLeftYStick), OIConstants::kDriveDeadband)},
            -units::meters_per_second_t{frc::ApplyDeadband(m_driverController.GetRawAxis(OIConstants::kLeftXStick), OIConstants::kDriveDeadband)},
            -units::radians_per_second_t{frc::ApplyDeadband(m_driverController.GetRawAxis(OIConstants::kRightXStick), OIConstants::kDriveDeadband)},
            true, 
            true, 
            m_driverController.GetRawButton(OIConstants::kRightStickButton),
            m_driverController2.GetRawButton(OIConstants::kBButton));
      },
      {&m_drive}));

    // Set up default intake command
    // The right trigger controls the intake in, and the left trigger controls the intake out
    m_launcher.SetDefaultCommand(frc2::RunCommand(
      [this] {
        m_launcher.intakeAndOutake(frc::ApplyDeadband(m_driverController2.GetRawAxis(OIConstants::kRigthTrigger), OIConstants::kIntakeDeadband), 
                                  frc::ApplyDeadband(m_driverController2.GetRawAxis(OIConstants::kLeftTrigger), OIConstants::kIntakeDeadband),
                                  m_driverController2.GetRawButton(OIConstants::kBButton));
      },
      {&m_launcher}));

    m_arm.SetDefaultCommand(frc2::RunCommand(
      [this] {
        m_arm.LockAngle(m_driverController2.GetRawButton(OIConstants::kAButton), 
                        m_driverController2.GetRawButton(OIConstants::kXButton), 
                        m_driverController2.GetRawButton(OIConstants::kYButton),
                        m_driverController2.GetRawButton(OIConstants::kBButton));
      },
      {&m_arm}));

    m_climber.SetDefaultCommand(frc2::RunCommand(
      [this] {
        m_climber.setVoltage(12, frc::ApplyDeadband(m_driverController2.GetRawAxis(OIConstants::kLeftYStick), OIConstants::kClimberDeadband));
      },
      {&m_climber}));
}

void RobotContainer::ConfigureButtonBindings() {
    // Set X position to avoid the robot from moving when pressing the left bumper
    frc2::JoystickButton(&m_driverController, OIConstants::kLeftBumper)
        .WhileTrue(new frc2::RunCommand([this] { m_drive.SetX(); }, {&m_drive}));
}

frc2::Command* RobotContainer::GetAutonomousCommand() {

  bool onlyShoot = false;
  bool shootAndMove = false;
  bool test = true;

  if(onlyShoot){
    shootAndMove = false;
    test = false;
  }else if(shootAndMove){
    onlyShoot = false;
    test = false;
  }else if (test){
    shootAndMove = false;
    onlyShoot = false;
  }

  if(shootAndMove){


    m_arm.MoveToAngle(kArmShootingAngle);
    m_launcher.autonomousShoot();

    frc::TrajectoryConfig configAutoPro(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration);

    configAutoPro.SetKinematics(m_drive.kDriveKinematics);

    auto autoPro = frc::TrajectoryGenerator::GenerateTrajectory(

    frc::Pose2d{0_m, 0_m, 0_deg},
      
    {frc::Translation2d{-1_m, 0_m}, frc::Translation2d{-2_m, 0_m}},

    frc::Pose2d{-2_m, 0_m, 0_deg},

    configAutoPro);



    frc::ProfiledPIDController<units::radians> thetaController{
      AutoConstants::kPThetaController, 0, 0,
      AutoConstants::kThetaControllerConstraints};

  thetaController.EnableContinuousInput(units::radian_t{-std::numbers::pi},
                                        units::radian_t{std::numbers::pi});

  frc2::SwerveControllerCommand<4> swerveControllerCommand(
      autoPro, [this]() { return m_drive.GetPose(); },

      m_drive.kDriveKinematics,

      frc::PIDController{AutoConstants::kPXController, 0, 0},
      frc::PIDController{AutoConstants::kPYController, 0, 0}, thetaController,

      [this](auto moduleStates) { m_drive.SetModuleStates(moduleStates); },

      {&m_drive});

  // Reset odometry to the starting pose of the trajectory.
  m_drive.ResetOdometry(autoPro.InitialPose());

  // no auto
  return new frc2::SequentialCommandGroup(
      std::move(swerveControllerCommand),
      frc2::InstantCommand(
          [this]() { m_drive.Drive(0_mps, 0_mps, 0_rad_per_s, false, false, false, false); },
          {}));

  }else if(onlyShoot){

    m_arm.MoveToAngle(kArmShootingAngle);
    m_launcher.autonomousShoot();

  }else if(test){

    frc::TrajectoryConfig configAutoPro(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration);

    configAutoPro.SetKinematics(m_drive.kDriveKinematics);

    auto autoPro = frc::TrajectoryGenerator::GenerateTrajectory(

    frc::Pose2d{0_m, 0_m, 0_deg},
      
    {frc::Translation2d{-1_m, 0_m}, frc::Translation2d{-2_m, 0_m}},

    frc::Pose2d{-2_m, 0_m, 0_deg},

    configAutoPro);



    frc::ProfiledPIDController<units::radians> thetaController{
      AutoConstants::kPThetaController, 0, 0,
      AutoConstants::kThetaControllerConstraints};

  thetaController.EnableContinuousInput(units::radian_t{-std::numbers::pi},
                                        units::radian_t{std::numbers::pi});

  frc2::SwerveControllerCommand<4> swerveControllerCommand(
      autoPro, [this]() { return m_drive.GetPose(); },

      m_drive.kDriveKinematics,

      frc::PIDController{AutoConstants::kPXController, 0, 0},
      frc::PIDController{AutoConstants::kPYController, 0, 0}, thetaController,

      [this](auto moduleStates) { m_drive.SetModuleStates(moduleStates); },

      {&m_drive});

  // Reset odometry to the starting pose of the trajectory.
  m_drive.ResetOdometry(autoPro.InitialPose());

  // no auto
  return new frc2::SequentialCommandGroup(
      std::move(swerveControllerCommand),
      frc2::InstantCommand(
          [this]() { m_drive.Drive(0_mps, 0_mps, 0_rad_per_s, false, false, false, false); },
          {}));

  }else{

  // Set up config for trajectory
  frc::TrajectoryConfig config(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration);
  // Add kinematics to ensure max speed is actually obeyed
  config.SetKinematics(m_drive.kDriveKinematics);

  // An example trajectory to follow.  All units in meters.
  auto exampleTrajectory = frc::TrajectoryGenerator::GenerateTrajectory(
      // Start at the origin facing the +X direction
      frc::Pose2d{0_m, 0_m, 0_deg},
      // Pass through these two interior waypoints, making an 's' curve path
      {frc::Translation2d{1_m, 1_m}, frc::Translation2d{2_m, -1_m}},
      // End 3 meters straight ahead of where we started, facing forward
      frc::Pose2d{3_m, 0_m, 0_deg},
      // Pass the config
      config);

  frc::ProfiledPIDController<units::radians> thetaController{
      AutoConstants::kPThetaController, 0, 0,
      AutoConstants::kThetaControllerConstraints};

  thetaController.EnableContinuousInput(units::radian_t{-std::numbers::pi},
                                        units::radian_t{std::numbers::pi});

  frc2::SwerveControllerCommand<4> swerveControllerCommand(
      exampleTrajectory, [this]() { return m_drive.GetPose(); },

      m_drive.kDriveKinematics,

      frc::PIDController{AutoConstants::kPXController, 0, 0},
      frc::PIDController{AutoConstants::kPYController, 0, 0}, thetaController,

      [this](auto moduleStates) { m_drive.SetModuleStates(moduleStates); },

      {&m_drive});

  // Reset odometry to the starting pose of the trajectory.
  m_drive.ResetOdometry(exampleTrajectory.InitialPose());

  // no auto
  return new frc2::SequentialCommandGroup(
      std::move(swerveControllerCommand),
      frc2::InstantCommand(
          [this]() { m_drive.Drive(0_mps, 0_mps, 0_rad_per_s, false, false, false, false); },
          {}));

  }
}