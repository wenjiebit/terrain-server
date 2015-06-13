#include <model/WholeBodyKinematics.h>
#include <model/WholeBodyDynamics.h>
#include <model/RobCoGenWholeBodyKinematics.h>
#include <model/RobCoGenWholeBodyDynamics.h>
#include <robot/HyLWholeBodyKinematics.h>
#include <robot/HyLWholeBodyDynamics.h>
#include <iit/rbd/rbd.h>
#include "ctime"

struct TimerInfo {
	/// time stamp when timer_start() gets called
	clock_t clock_start_value;

	/// time stamp when the timer was stopped
	clock_t clock_end_value;

	/// duration between clock_start_value and clock_end_value in seconds
	double duration_sec;
};

inline void timer_start (TimerInfo *timer) {
	timer->clock_start_value = clock();
}

inline double timer_stop (TimerInfo *timer) {
	timer->clock_end_value = clock();

	timer->duration_sec = static_cast<double>(timer->clock_end_value - timer->clock_start_value) * 1 / CLOCKS_PER_SEC;

	return timer->duration_sec;
}


int main(int argc, char **argv)
{
	dwl::model::WholeBodyKinematics kin;
	std::string model_file = "/home/cmastalli/ros_workspace/src/dwl/thirdparty/rbdl/hyl_fbtest.urdf";
	kin.modelFromURDF(model_file);
	dwl::model::WholeBodyDynamics dyn;
	dyn.modelFromURDF(model_file, true);

	dwl::model::RobCoGenWholeBodyKinematics* kin_ptr = new dwl::robot::HyLWholeBodyKinematics();
	dwl::model::RobCoGenWholeBodyDynamics* dyn_ptr = new dwl::robot::HyLWholeBodyDynamics();
	dyn_ptr->setKinematicModel(kin_ptr);

	iit::rbd::Vector6D base_wrench, base_pos, base_vel, base_acc, g;
	Eigen::VectorXd joint_forces(2), joint_pos(2), joint_pos2(1), joint_vel(2), joint_vel2(1), joint_acc(2), joint_acc2(1);
	base_pos = dwl::rbd::Vector6d::Zero();
	base_vel = dwl::rbd::Vector6d::Zero();
	base_acc = dwl::rbd::Vector6d::Zero();
	joint_pos << 0., 0.;//0.504653, -1.45204;
	joint_pos2 << 0.;//, 0., 0.;//0.504653, -1.45204;
	joint_vel << 0., 1.;
	joint_vel2 << 0.;//, 0., 0.;
	joint_acc << 0., 1.;
	joint_acc2 << 0.;//, 0., 0.;

	// Testing kinematics
	kin_ptr->updateState(base_pos, joint_pos);

	dwl::rbd::EndEffectorSelector effector_set;
//	effector_set["foot"] = true;
	kin.addEndEffector("trunk");
//	kin.addEndEffector("upperleg");
//	kin.addEndEffector("test");
//	kin.addEndEffector("foot");
//	effector_set["lf_foot"] = true;
//	effector_set["rh_foot"] = true;
//	kin.addEndEffector("lf_foot");
//	kin.addEndEffector("rh_foot");
	Eigen::MatrixXd jacobian;

//	kin.computeWholeBodyJacobian(jacobian, base_pos, joint_pos2);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << jacobian << " = Body jac" << std::endl;

//	Eigen::MatrixXd floating_jac;
//	kin.getFloatingBaseJacobian(floating_jac, jacobian);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << floating_jac << " = Floating base jac" << std::endl;
//
//	Eigen::MatrixXd fixed_jac;
//	kin.getFixedBaseJacobian(fixed_jac, jacobian);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << fixed_jac << " = Fixed base jac" << std::endl;

//	kin_ptr->computeFixedBaseJacobian(jacobian, joint_pos, effector_set, dwl::Full);
//	double duration2 = timer_stop (&tinfo2);
//	std::cout << duration2 << " = time duration" << std::endl;
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << jacobian << " = MarcoF jac" << std::endl;
//
//	Eigen::VectorXd op_pos;
//	kin.computeWholeBodyFK(op_pos, base_pos, joint_pos2, effector_set, dwl::rbd::Full);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << op_pos << " = Body tf" << std::endl;
//
//	Eigen::VectorXd position;
//	kin_ptr->computeEffectorFK(position, joint_pos, dwl::model::Linear);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << position << " = MarcoF tf" << std::endl;

//	Eigen::VectorXd velocity;
//	kin.computeWholeBodyVelocity(velocity, base_pos, joint_pos2, base_vel, joint_vel2, effector_set, dwl::Full);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << velocity << " = Body vel" << std::endl;
//
//	kin_ptr->opVelocityFromJointSpace(velocity, base_pos, joint_pos, base_vel, joint_vel, effector_set, dwl::Full);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << velocity << " = robcogen Body vel" << std::endl;

//	Eigen::VectorXd acceleration;
//	kin.computeWholeBodyAcceleration(acceleration, base_pos, joint_pos2, base_vel, joint_vel2, base_acc, joint_acc2, effector_set, dwl::Full);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << acceleration << " = Body acc" << std::endl;


//	dwl::rbd::EndEffectorPosition op_pos2;
//	op_pos2["trunk"] = Eigen::Vector3d::Zero();
//	op_pos2["foot"] = op_pos.tail(3);
//	Eigen::VectorXd op_pos2 = Eigen::VectorXd::Zero(6);
//	kin.addEndEffector("trunk");
//	dwl::rbd::EndEffectorSelector effector_set2;
//	effector_set2["trunk"] = true;
//	effector_set2["foot"] = true;
//	op_pos2 << op_pos.tail(3), 0, 0, 0.0;
//	std::cout << op_pos2.transpose() << std::endl;
//	Eigen::Vector3d init;
//	init << 0, 0.2, -1;
//	TimerInfo tinfo;
//	timer_start (&tinfo);
//	for (int i = 0; i < 100000; i++)
//		kin.computeWholeBodyIK(base_pos, joint_pos, dwl::Vector6d::Zero(), init, op_pos2, effector_set2);
//	double duration = timer_stop (&tinfo);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << duration << " = time duration" << std::endl;
//	std::cout << base_pos.transpose() << " | " << joint_pos.transpose() << " = Body ik" << std::endl;



//	Eigen::VectorXd jacd_qd;
//	kin.computeWholeBodyJdotQdot(jacd_qd, base_pos, joint_pos2, base_vel, joint_vel2, effector_set, dwl::rbd::Full);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << jacd_qd.transpose() << " = jacd_qd" << std::endl;

//	Eigen::VectorXd jacd_qd2;
//	dyn_ptr->opAccelerationContributionFromJointVelocity(jacd_qd2, base_pos, joint_pos, base_vel, joint_vel, effector_set);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << jacd_qd2.transpose() << " = Michi jacd_qd" << std::endl;

	dwl::rbd::EndEffectorForce fext;
	dwl::rbd::Vector6d forc;
	forc << 0,0,0,0,0,10;//67;
	fext["trunk"] = forc;
//	fext["foot"] = forc;
//	fext["lowerleg"] = forc;



//	base_pos(dwl::rbd::AX) = 1.57;
//	base_pos(dwl::rbd::AY) = 1.57;
	//base_pos(dwl::rbd::AZ) = 3;
	base_pos(dwl::rbd::LX) = 10;
	base_pos(dwl::rbd::LY) = 20;
	base_pos(dwl::rbd::LZ) = 30;
//	base_acc(dwl::rbd::LZ) = -9.81;
	base_vel(dwl::rbd::LX) = 1;
	base_vel(dwl::rbd::LZ) = 2;
	joint_vel2 << 1;
	joint_acc2 << -9.81;

	dyn.computeWholeBodyInverseDynamics(base_wrench, joint_forces, base_pos, joint_pos2, base_vel, joint_vel2, base_acc, joint_acc2, fext);
	std::cout << "---------------------------------------" << std::endl;
	std::cout << base_wrench.transpose() << " | " << joint_forces.transpose() << " = tau" << std::endl;

//	dyn_ptr->computeWholeBodyInverseDynamics(base_wrench, joint_forces, dwl::rbd::Vector6d::Zero(), base_pos, joint_pos, base_vel, joint_vel, base_acc, joint_acc);
//	std::cout << "---------------------------------------" << std::endl;
//	std::cout << base_wrench.transpose() << " | " << joint_forces.transpose() << " = robcogen tau" << std::endl;
	dyn.computeWholeBodyInverseDynamics(base_acc, joint_forces, base_pos, joint_pos2, base_vel, joint_vel2, joint_acc2, fext);


	std::cout << "Base acc = " << base_acc.transpose() << std::endl;
	std::cout << "Base wrench = " << base_wrench.transpose() << std::endl;
	std::cout << "Joint forces = " << joint_forces.transpose() << std::endl;

    return 0;
}
