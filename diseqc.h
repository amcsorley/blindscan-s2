/* diseqc -- simple diseqc commands for the Linux DVB S2 API
 *
 * UDL (updatelee@gmail.com)
 * Derived from work by:
 * 	Igor M. Liplianin (liplianin@me.by)
 * 	Alex Betis <alex.betis@gmail.com>
 *	Pendragon
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/dvb/frontend.h>

#define MOTOR_WAIT_TIME 45

extern void diseqc_send_msg (int fefd, struct dvb_diseqc_master_cmd cmd, unsigned int verbose);
extern void setup_switch (int fefd, fe_sec_voltage_t voltage, fe_sec_tone_mode_t tone,
	int committed, int uncommitted, unsigned int verbose);
extern void motor_usals(int fefd, double site_lat, double site_long, double sat_long,
	unsigned int verbose, int motor_wait_time);
extern double radian( double number );
extern double degree( double number );
extern void motor_dir(int fefd, int dir, unsigned int verbose);
