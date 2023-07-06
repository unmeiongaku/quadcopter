/*
 * LPV_cont_discrete.c
 *
 *  Created on: Jun 26, 2023
 *      Author: nguye
 */

#include "params.h"
#include "../Rotational_matrix/rotational_matrix.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int lpv_cont_discrete(double *roll_t,double *pitch_t,double *yaw_t,double *states_u,double *states_v,double *states_w,double *states_p,double *states_q,double *states_r,double *states_phi,double *states_theta,double *states_psi,double *Omega_total,double Ad_matrix[6][6],double Bd_matrix[6][3],double Cd_matrix[3][6], double Dd_matrix[3][3]){
	double R_matrix_t[3][3],T_matrix_t[3][3];
	double euler_t[3],states_t[9];
	double x_dot_t,y_dot_t,z_dot_t;
	euler_t[0] = *roll_t; euler_t[1] = *pitch_t; euler_t[2] =*yaw_t;
	states_t[0] = *states_u; states_t[1] = *states_v; states_t[2] = *states_w;
	states_t[3] = *states_p; states_t[4] = *states_q; states_t[5] = *states_r;
	states_t[6] = *states_phi; states_t[7] = *states_theta; states_t[8] = *states_psi;
	get_rotational_matrix_lpv_cont_discrete(&euler_t[0],&euler_t[1],&euler_t[2],R_matrix_t,T_matrix_t,&states_t[0],&states_t[1],&states_t[2],&states_t[3],&states_t[4],&states_t[5],&x_dot_t,&y_dot_t,&z_dot_t);
	double rot_vel_body[3];
	rot_vel_body[0] = states_t[3];
	rot_vel_body[1] = states_t[4];
	rot_vel_body[2] = states_t[5];
	/*Calculation rot_vel_fixed by multiplication T_maxtrix_t with rot_vel_body*/
	double rot_vel_fix[3];
	rot_vel_fix[0] = T_matrix_t[0][0]*rot_vel_body[0]+T_matrix_t[0][1]*rot_vel_body[1]+T_matrix_t[0][2]*rot_vel_body[2];
	rot_vel_fix[1] = T_matrix_t[1][0]*rot_vel_body[0]+T_matrix_t[1][1]*rot_vel_body[1]+T_matrix_t[1][2]*rot_vel_body[2];
	rot_vel_fix[2] = T_matrix_t[2][0]*rot_vel_body[0]+T_matrix_t[2][1]*rot_vel_body[1]+T_matrix_t[2][2]*rot_vel_body[2];
	double phi_dot,theta_dot,psi_dot;
	phi_dot = rot_vel_fix[0];
	theta_dot = rot_vel_fix[1];
	psi_dot = rot_vel_fix[2];
	phi_dot = phi_dot;
	theta_dot = theta_dot;
	psi_dot = psi_dot;
	/*Create the continuous LPV A, B, C, D matrices*/
	double A_matrix[6][6],B_matrix[6][3];
	uint8_t C_matrix[6][6];
	uint8_t D_matrix[3][3];
	/*Create A matrix*/
	A_matrix[0][0]=0;	A_matrix[0][1]=1;																			A_matrix[0][2]=0;	A_matrix[0][3]=0;																			A_matrix[0][4]=0;	A_matrix[0][5]=0;
	A_matrix[1][0]=0;	A_matrix[1][1]=0;																			A_matrix[1][2]=0;	A_matrix[1][3]=-*Omega_total*(params.MPC_Cons_Jtp/params.MPC_Cons_Ix);						A_matrix[1][4]=0;	A_matrix[1][5]=theta_dot*((params.MPC_Cons_Iy-params.MPC_Cons_Iz)/params.MPC_Cons_Ix);
	A_matrix[2][0]=0;	A_matrix[2][1]=0;																			A_matrix[2][2]=0;	A_matrix[2][3]=1;																			A_matrix[2][4]=0;	A_matrix[2][5]=0;
	A_matrix[3][0]=0;	A_matrix[3][1]=*Omega_total*(params.MPC_Cons_Jtp/params.MPC_Cons_Iy);						A_matrix[3][2]=0;	A_matrix[3][3]=0;																			A_matrix[3][4]=0;	A_matrix[3][5]=phi_dot*((params.MPC_Cons_Iz-params.MPC_Cons_Ix)/params.MPC_Cons_Iy);
	A_matrix[4][0]=0;	A_matrix[4][1]=0;																			A_matrix[4][2]=0;	A_matrix[4][3]=0;																			A_matrix[4][4]=0;	A_matrix[4][5]=1;
	A_matrix[5][0]=0;	A_matrix[5][1]=(theta_dot/2)*((params.MPC_Cons_Ix-params.MPC_Cons_Iy)/params.MPC_Cons_Iz);	A_matrix[5][2]=0;	A_matrix[5][3]=(phi_dot/2)*((params.MPC_Cons_Ix-params.MPC_Cons_Iy)/params.MPC_Cons_Iz);	A_matrix[5][4]=0;	A_matrix[5][5]=0;
	/*Create B matrix*/
	B_matrix[0][0]=0;						B_matrix[0][1]=0;							B_matrix[0][2]=0;
	B_matrix[1][0]=1/params.MPC_Cons_Ix;	B_matrix[1][1]=0;							B_matrix[1][2]=0;
	B_matrix[2][0]=0;						B_matrix[2][1]=0;							B_matrix[2][2]=0;
	B_matrix[3][0]=0;						B_matrix[3][1]=1/params.MPC_Cons_Iy;		B_matrix[3][2]=0;
	B_matrix[4][0]=0;						B_matrix[4][1]=0;							B_matrix[4][2]=0;
	B_matrix[5][0]=0;						B_matrix[5][1]=0;							B_matrix[4][2]=1/params.MPC_Cons_Iz;
	/*Create C matrix*/
	C_matrix[0][0]=1;	C_matrix[0][1]=0;	C_matrix[0][2]=0;	C_matrix[0][3]=0;	C_matrix[0][4]=0;	C_matrix[0][5]=0;
	C_matrix[1][0]=0;	C_matrix[1][1]=0;	C_matrix[1][2]=1;	C_matrix[1][3]=0;	C_matrix[1][4]=0;	C_matrix[1][5]=0;
	C_matrix[2][0]=0;	C_matrix[2][1]=0;	C_matrix[2][2]=0;	C_matrix[2][3]=0;	C_matrix[2][4]=1;	C_matrix[2][5]=0;
	/*Create D matrix*/
	D_matrix[0][0]=0;	D_matrix[0][1]=0;	D_matrix[0][2]=0;
	D_matrix[1][0]=0;	D_matrix[1][1]=0;	D_matrix[1][2]=0;
	D_matrix[2][0]=0;	D_matrix[2][1]=0;	D_matrix[2][2]=0;
	/* Discretize the system (Forward Euler) */
	//size cols of A = 6
	// Discretize A_matrix
	/*Ad=np.identity(np.size(A,1))+Ts*A*/
	//np.identity(np.size(A,1))
	for(int i=0;i<6;i++){
		for(int j=0;i<6;j++){
			Ad_matrix[i][j] = 0;
		}
	}
	for(int i=0;i<6;i++){
		Ad_matrix[i][i]=1;
	}
	for(int i=0;i<6;i++){
		for(int j=0;j<6;j++){
			Ad_matrix[i][j] += params.MPC_Cons_Ts*A_matrix[i][j];
		}
	}
	// Discretize B_Matrix
	for(int i=0;i<6;i++){
		for(int j=0;j<3;j++){
			Bd_matrix[i][j] *= params.MPC_Cons_Ts*B_matrix[i][j];
		}
	}
	// Discretize C_Matrix
	for(int i=0;i<3;i++){
		for(int j=0;j<6;j++){
			Cd_matrix[i][j] = C_matrix[i][j];
		}
	}
	// Discretize D_Matrix
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			Dd_matrix[i][j] = D_matrix[i][j];
		}
	}
	return 0;
}
