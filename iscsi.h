/*
 * RFC 3720 (iSCSI) protocol data types
 * Copyright (C) 2004 Dmitry Yusupov, Alex Aizman
 * maintained by open-iscsi@@googlegroups.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * See the file COPYING included with this distribution for more details.
 */

#ifndef ISCSI_H
#define ISCSI_H

/*
 * useful common(control and data pathes) macro
 */
#define ntoh24(p) (((p)[0] << 16) | ((p)[1] << 8) | ((p)[2]))
#define hton24(p, v) { \
        p[0] = (((v) >> 16) & 0xFF); \
        p[1] = (((v) >> 8) & 0xFF); \
        p[2] = ((v) & 0xFF); \
}
#define zero_data(p) {p[0]=0;p[1]=0;p[2]=0;}

/*
 * iSCSI Template Message Header
 */
typedef struct iscsi_hdr {
	uint8_t		opcode;
	uint8_t		flags;		/* Final bit */
	uint8_t		rsvd2[2];
	uint8_t		hlength;        /* AHSs total length */
	uint8_t		dlength[3];     /* Data length */
	uint8_t		lun[8];
	uint32_t	itt;		/* Initiator Task Tag */
	uint32_t	ttt;		/* Target Task Tag */
	uint32_t	statsn;
	uint32_t	exp_statsn;
	uint8_t		other[16];
} iscsi_hdr_t;

/************************* RFC 3720 Begin *****************************/

#define ISCSI_RESERVED_TAG		0xffffffff

/* Opcode encoding bits */
#define ISCSI_OP_RETRY		        0x80
#define ISCSI_OP_IMMEDIATE		0x40
#define ISCSI_OPCODE_MASK		0x3F

/* Initiator Opcode values */
#define ISCSI_OP_NOOP_OUT		0x00
#define ISCSI_OP_SCSI_CMD		0x01
#define ISCSI_OP_SCSI_TMFUNC		0x02
#define ISCSI_OP_LOGIN			0x03
#define ISCSI_OP_TEXT			0x04
#define ISCSI_OP_SCSI_DATA_OUT		0x05
#define ISCSI_OP_LOGOUT			0x06
#define ISCSI_OP_SNACK			0x10

/* Target Opcode values */
#define ISCSI_OP_NOOP_IN		0x20
#define ISCSI_OP_SCSI_CMD_RSP		0x21
#define ISCSI_OP_SCSI_TMFUNC_RSP	0x22
#define ISCSI_OP_LOGIN_RSP		0x23
#define ISCSI_OP_TEXT_RSP		0x24
#define ISCSI_OP_SCSI_DATA_IN	        0x25
#define ISCSI_OP_LOGOUT_RSP		0x26
#define ISCSI_OP_R2T			0x31
#define ISCSI_OP_ASYNC_EVENT		0x32
#define ISCSI_OP_REJECT_MSG		0x3f

/* SCSI Command Header */
typedef struct iscsi_cmd {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2;
	uint8_t cmdrn;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t data_length;
	uint32_t cmdsn;
	uint32_t exp_statsn;
	uint8_t cdb[16];	/* SCSI Command Block */
	/* Additional Data (Command Dependent) */
} iscsi_cmd_t;

/* Command PDU flags */
#define ISCSI_FLAG_CMD_FINAL		0x80
#define ISCSI_FLAG_CMD_READ		0x40
#define ISCSI_FLAG_CMD_WRITE		0x20
#define ISCSI_FLAG_CMD_ATTR_MASK	0x07	/* 3 bits */

/* SCSI Command Attribute values */
#define ISCSI_ATTR_UNTAGGED		0
#define ISCSI_ATTR_SIMPLE		1
#define ISCSI_ATTR_ORDERED		2
#define ISCSI_ATTR_HEAD_OF_QUEUE	3
#define ISCSI_ATTR_ACA			4

/* SCSI Response Header */
typedef struct iscsi_cmd_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t response;
	uint8_t cmd_status;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t rsvd1;
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint32_t expdatasn;
	uint32_t residual_count;
	uint32_t bi_residual_count;
	/* Response or Sense Data (optional) */
} iscsi_cmd_rsp_t;

/* Command Response PDU flags */
#define ISCSI_FLAG_CMD_BIDI_OVERFLOW        0x10
#define ISCSI_FLAG_CMD_BIDI_UNDERFLOW       0x08
#define ISCSI_FLAG_CMD_OVERFLOW             0x04
#define ISCSI_FLAG_CMD_UNDERFLOW            0x02

/* iSCSI Status values. Valid if Rsp Selector bit is not set */
#define ISCSI_STATUS_CMD_COMPLETED	0
#define ISCSI_STATUS_TARGET_FAILURE	1
#define ISCSI_STATUS_SUBSYS_FAILURE	2

/* Asynchronous Event Header */
typedef struct iscsi_async {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t rsvd3;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint8_t rsvd4[8];
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint8_t async_event;
	uint8_t async_vcode;
	uint16_t param1;
	uint16_t param2;
	uint16_t param3;
	uint8_t rsvd5[4];
} iscsi_async_t;

/* iSCSI Event Indicator values */
#define ASYNC_EVENT_SCSI_EVENT                  0
#define ASYNC_EVENT_REQUEST_LOGOUT              1
#define ASYNC_EVENT_DROPPING_CONNECTION         2
#define ASYNC_EVENT_DROPPING_ALL_CONNECTIONS	3
#define ASYNC_EVENT_PARAM_NEGOTIATION	        4
#define ASYNC_EVENT_VENDOR_SPECIFIC             255

/* NOP-Out Message */
typedef struct iscsi_nopout {
	uint8_t opcode;
	uint8_t flags;
	uint16_t rsvd2;
	uint8_t rsvd3;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t ttt;	/* Target Transfer Tag */
	uint32_t cmdsn;
	uint32_t exp_statsn;
	uint8_t rsvd4[16];
} iscsi_nopout_t;

/* NOP-In Message */
typedef struct iscsi_nopin {
	uint8_t opcode;
	uint8_t flags;
	uint16_t rsvd2;
	uint8_t rsvd3;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t ttt;	/* Target Transfer Tag */
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint8_t rsvd4[12];
} iscsi_nopin_t;

/* SCSI Task Management Message Header */
typedef struct iscsi_tm {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd1[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t rtt;	/* Reference Task Tag */
	uint32_t cmdsn;
	uint32_t exp_statsn;
	uint32_t refcmdsn;
	uint32_t expdatasn;
	uint8_t rsvd2[8];
} iscsi_tm_t;

#define ISCSI_FLAG_TASK_MGMT_FUNCTION_MASK  0x7F

/* Function values */
#define ISCSI_TM_FUNC_ABORT_TASK         1
#define ISCSI_TM_FUNC_ABORT_TASK_SET     2
#define ISCSI_TM_FUNC_CLEAR_ACA          3
#define ISCSI_TM_FUNC_CLEAR_TASK_SET     4
#define ISCSI_TM_FUNC_LOGICAL_UNIT_RESET 5
#define ISCSI_TM_FUNC_TARGET_WARM_RESET  6
#define ISCSI_TM_FUNC_TARGET_COLD_RESET  7
#define ISCSI_TM_FUNC_TASK_REASSIGN	 8

/* SCSI Task Management Response Header */
typedef struct iscsi_tm_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t response;	/* see Response values below */
	uint8_t qualifier;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd2[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t rtt;	/* Reference Task Tag */
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint8_t rsvd3[12];
} iscsi_tm_rsp_t;

/* Response values */
#define SCSI_TCP_TM_RESP_COMPLETE	0x00
#define SCSI_TCP_TM_RESP_NO_TASK	0x01
#define SCSI_TCP_TM_RESP_NO_LUN		0x02
#define SCSI_TCP_TM_RESP_TASK_ALLEGIANT	0x03
#define SCSI_TCP_TM_RESP_NO_FAILOVER	0x04
#define SCSI_TCP_TM_RESP_IN_PRGRESS	0x05
#define SCSI_TCP_TM_RESP_REJECTED	0xff

/* Ready To Transfer Header */
typedef struct iscsi_r2t_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t	hlength;
	uint8_t	dlength[3];
	uint8_t lun[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t ttt;	/* Target Transfer Tag */
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint32_t rttsn;
	uint32_t data_offset;
	uint32_t data_length;
} iscsi_r2t_rsp_t;

/* SCSI Data Hdr */
typedef struct iscsi_data {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t rsvd3;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint32_t itt;
	uint32_t ttt;
	uint32_t rsvd4;
	uint32_t exp_statsn;
	uint32_t rsvd5;
	uint32_t datasn;
	uint32_t offset;
	uint32_t rsvd6;
	/* Payload */
} iscsi_data_t;

/* SCSI Data Response Hdr */
typedef struct iscsi_data_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2;
	uint8_t cmd_status;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t lun[8];
	uint32_t itt;
	uint32_t ttt;
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint32_t datasn;
	uint32_t offset;
	uint32_t residual_count;
} iscsi_data_rsp_t;

/* Data Response PDU flags */
#define ISCSI_FLAG_DATA_ACK                 0x40
#define ISCSI_FLAG_DATA_OVERFLOW            0x04
#define ISCSI_FLAG_DATA_UNDERFLOW           0x02
#define ISCSI_FLAG_DATA_STATUS              0x01

/* Text Header */
typedef struct iscsi_text {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd4[8];
	uint32_t itt;
	uint32_t ttt;
	uint32_t cmdsn;
	uint32_t exp_statsn;
	uint8_t rsvd5[16];
	/* Text - key=value pairs */
} iscsi_text_t;

#define ISCSI_FLAG_TEXT_CONTINUE            0x40

/* Text Response Header */
typedef struct iscsi_text_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd4[8];
	uint32_t itt;
	uint32_t ttt;
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint8_t rsvd5[12];
	/* Text Response - key:value pairs */
} iscsi_text_rsp_t;

/* Login Header */
typedef struct iscsi_login {
	uint8_t opcode;
	uint8_t flags;
	uint8_t max_version;	/* Max. version supported */
	uint8_t min_version;	/* Min. version supported */
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t isid[6];	/* Initiator Session ID */
	uint16_t tsih;	/* Target Session Handle */
	uint32_t itt;	/* Initiator Task Tag */
	uint16_t cid;
	uint16_t rsvd3;
	uint32_t cmdsn;
	uint32_t exp_statsn;
	uint8_t rsvd5[16];
} iscsi_login_t;

/* Login PDU flags */
#define ISCSI_FLAG_LOGIN_TRANSIT            0x80
#define ISCSI_FLAG_LOGIN_CONTINUE           0x40
#define ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK 0x0C	/* 2 bits */
#define ISCSI_FLAG_LOGIN_NEXT_STAGE_MASK    0x03	/* 2 bits */

#define ISCSI_LOGIN_CURRENT_STAGE(flags) \
((flags & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK) >> 2)
#define ISCSI_LOGIN_NEXT_STAGE(flags)     \
(flags & ISCSI_FLAG_LOGIN_NEXT_STAGE_MASK)

/* Login Response Header */
typedef struct iscsi_login_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t max_version;	/* Max. version supported */
	uint8_t active_version;	/* Active version */
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t isid[6];	/* Initiator Session ID */
	uint16_t tsih;	/* Target Session Handle */
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t rsvd3;
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint8_t status_class;	/* see Login RSP ststus classes below */
	uint8_t status_detail;	/* see Login RSP Status details below */
	uint8_t rsvd4[10];
} iscsi_login_rsp_t;

/* Login stage (phase) codes for CSG, NSG */
#define ISCSI_INITIAL_LOGIN_STAGE		-1
#define ISCSI_SECURITY_NEGOTIATION_STAGE	0
#define ISCSI_OP_PARMS_NEGOTIATION_STAGE	1
#define ISCSI_FULL_FEATURE_PHASE		3

/* Login Status response classes */
#define STATUS_CLASS_SUCCESS		0x00
#define STATUS_CLASS_REDIRECT		0x01
#define STATUS_CLASS_INITIATOR_ERR	0x02
#define STATUS_CLASS_TARGET_ERR		0x03

/* Login Status response detail codes */
/* Class-0 (Success) */
#define ISCSI_LOGIN_STATUS_ACCEPT		0x00

/* Class-1 (Redirection) */
#define ISCSI_LOGIN_STATUS_TGT_MOVED_TEMP	0x01
#define ISCSI_LOGIN_STATUS_TGT_MOVED_PERM	0x02

/* Class-2 (Initiator Error) */
#define ISCSI_LOGIN_STATUS_INIT_ERR		0x00
#define ISCSI_LOGIN_STATUS_AUTH_FAILED		0x01
#define ISCSI_LOGIN_STATUS_TGT_FORBIDDEN	0x02
#define ISCSI_LOGIN_STATUS_TGT_NOT_FOUND	0x03
#define ISCSI_LOGIN_STATUS_TGT_REMOVED		0x04
#define ISCSI_LOGIN_STATUS_NO_VERSION		0x05
#define ISCSI_LOGIN_STATUS_ISID_ERROR		0x06
#define ISCSI_LOGIN_STATUS_MISSING_FIELDS	0x07
#define ISCSI_LOGIN_STATUS_CONN_ADD_FAILED	0x08
#define ISCSI_LOGIN_STATUS_NO_SESSION_TYPE	0x09
#define ISCSI_LOGIN_STATUS_NO_SESSION           0x0a
#define ISCSI_LOGIN_STATUS_INVALID_REQUEST      0x0b

/* Class-3 (Target Error) */
#define ISCSI_LOGIN_STATUS_TARGET_ERROR		0x00
#define ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE	0x01
#define ISCSI_LOGIN_STATUS_NO_RESOURCES	        0x02

/* Logout Header */
typedef struct iscsi_logout {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd1[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd2[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint16_t cid;
	uint8_t rsvd3[2];
	uint32_t cmdsn;
	uint32_t exp_statsn;
	uint8_t rsvd4[16];
} iscsi_logout_t;

/* Logout PDU flags */
#define ISCSI_FLAG_LOGOUT_REASON_MASK       0x7F

/* logout reason_code values */

#define ISCSI_LOGOUT_REASON_CLOSE_SESSION	0
#define ISCSI_LOGOUT_REASON_CLOSE_CONNECTION	1
#define ISCSI_LOGOUT_REASON_RECOVERY		2
#define ISCSI_LOGOUT_REASON_AEN_REQUEST	        3

/* Logout Response Header */
typedef struct iscsi_logout_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t response;	/* see Logout response values below */
	uint8_t rsvd2;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd3[8];
	uint32_t itt;	/* Initiator Task Tag */
	uint32_t rsvd4;
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint32_t rsvd5;
	uint16_t t2wait;
	uint16_t t2retain;
	uint32_t rsvd6;
} iscsi_logout_rsp_t;

/* logout response status values */

#define ISCSI_LOGOUT_SUCCESS		  0
#define ISCSI_LOGOUT_CID_NOT_FOUND	  1
#define ISCSI_LOGOUT_RECOVERY_UNSUPPORTED 2
#define ISCSI_LOGOUT_CLEANUP_FAILED	  3

/* SNACK Header */
typedef struct iscsi_snack {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[14];
	uint32_t itt;
	uint32_t begrun;
	uint32_t runlength;
	uint32_t exp_statsn;
	uint32_t rsvd3;
	uint32_t expdatasn;
	uint8_t rsvd6[8];
} iscsi_snack_t;

/* SNACK PDU flags */
#define ISCSI_FLAG_SNACK_TYPE_MASK          0x0F	/* 4 bits */

/* Reject Message Header */
typedef struct iscsi_reject_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t reason;
	uint8_t rsvd2;
	uint8_t rsvd3;
	uint8_t dlength[3];
	uint8_t rsvd4[16];
	uint32_t statsn;
	uint32_t exp_cmdsn;
	uint32_t max_cmdsn;
	uint32_t datasn;
	uint8_t rsvd5[8];
	/* Text - Rejected hdr */
} iscsi_reject_rsp_t;

/* Reason for Reject */
#define CMD_BEFORE_LOGIN        1
#define DATA_DIGEST_ERROR       2
#define DATA_SNACK_REJECT       3
#define ISCSI_PROTOCOL_ERROR    4
#define CMD_NOT_SUPPORTED       5
#define IMM_CMD_REJECT          6
#define TASK_IN_PROGRESS        7
#define INVALID_SNACK           8
#define BOOKMARK_REJECTED       9
#define BOOKMARK_NO_RESOURCES   10
#define NEGOTIATION_RESET	11

/************************* RFC 3720 End *****************************/

#endif /* ISCSI_H */