/*
 * rotation_matrix.h
 *
 *  Created on: May 27, 2023
 *      Author: nguye
 */

#ifndef APPLICATION_MPC_ROTATIONAL_MATRIX_ROTATIONAL_MATRIX_H_
#define APPLICATION_MPC_ROTATIONAL_MATRIX_ROTATIONAL_MATRIX_H_

#include <stdint.h>
#include <stdbool.h>
#include "bno055.h"

int get_rotational_matrix_pos_controller(double R_matrix[3][3], double *state_u, double *state_v, double *state_w, double *state_phi, double *state_theta, double *state_psi, double *x_dot, double *y_dot, double *z_dot);
int get_rotational_matrix_lpv_cont_discrete(double R_matrix[3][3],double T_maxtrix[3][3],double *state_u,double *state_v,double *state_w,double *state_p,double *state_q,double *state_r,double *state_phi, double *state_theta, double *state_psi, double *x_dot, double *y_dot, double *z_dot);
#endif /* APPLICATION_MPC_ROTATIONAL_MATRIX_ROTATIONAL_MATRIX_H_ */
