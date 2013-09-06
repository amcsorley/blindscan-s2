/* blindscan-s2 -- blindscan tool for the Linux DVB S2 API
 *
 * chinesebob@gmail.com
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

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/dvb/frontend.h>
#include "diseqc.h"

#define SLEEP_TIME 20000
#define HORIZONTAL SEC_VOLTAGE_18
#define VERTICAL SEC_VOLTAGE_13
#define NOPOLARITY SEC_VOLTAGE_OFF
#define SCAN_H 1
#define SCAN_V 1
#define SCAN_N 0
#define FEC 9
#define TONE SEC_TONE_OFF
#define CBAND_LOF 5150
#define INTERACTIVE 0
#define MONITOR 0
#define MONITOR_RETUNE 0
#define DELSYS 0
#define RETUNE 1
#define ADAPTER 0
#define FRONTEND 0
#define RUN_BLINDSCAN 0
#define VERBOSE 0
#define FREQ_MULT 1000
#define STARTFREQ 950
#define ENDFREQ 1450
#define GOODFREQ 0
#define LBAND_MAX 2150
#define SYMRATE 1000
#define STEP 20
#define LOF 0
#define COMMITTED 0
#define UNCOMMITTED 0
#define SITE_LAT 0
#define SITE_LONG 0
#define SAT_LONG 0
#define FEDEV "/dev/dvb/adapter%d/frontend%d"

