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
#include "timer.h"
#include "bno055.h"

int get_rotational_matrix_pos_controller(float *roll, float *pitch,float *yaw, float *R_matrix[3][3],float *state_u,float *state_v,float *state_w, float *x_dot, float *y_dot, float *z_dot);
int get_rotational_matrix_lpv_cont_discrete(float *roll, float *pitch,float *yaw, float *R_matrix[3][3],float *T_maxtrix[3][3],float *state_u,float *state_v,float *state_w,float *state_p,float *state_q,float *state_r, float *x_dot, float *y_dot, float *z_dot);
#endif /* APPLICATION_MPC_ROTATIONAL_MATRIX_ROTATIONAL_MATRIX_H_ */
