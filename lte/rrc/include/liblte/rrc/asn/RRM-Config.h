/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-InterNodeDefinitions"
 * 	found in "EUTRA-InterNodeDefinitions.asn"
 */

#ifndef	_RRM_Config_H_
#define	_RRM_Config_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include <liblte/rrc/asn/NativeEnumerated.h>
#include <liblte/rrc/asn/constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ue_InactiveTime {
	ue_InactiveTime_s1	= 0,
	ue_InactiveTime_s2	= 1,
	ue_InactiveTime_s3	= 2,
	ue_InactiveTime_s5	= 3,
	ue_InactiveTime_s7	= 4,
	ue_InactiveTime_s10	= 5,
	ue_InactiveTime_s15	= 6,
	ue_InactiveTime_s20	= 7,
	ue_InactiveTime_s25	= 8,
	ue_InactiveTime_s30	= 9,
	ue_InactiveTime_s40	= 10,
	ue_InactiveTime_s50	= 11,
	ue_InactiveTime_min1	= 12,
	ue_InactiveTime_min1s20c	= 13,
	ue_InactiveTime_min1s40	= 14,
	ue_InactiveTime_min2	= 15,
	ue_InactiveTime_min2s30	= 16,
	ue_InactiveTime_min3	= 17,
	ue_InactiveTime_min3s30	= 18,
	ue_InactiveTime_min4	= 19,
	ue_InactiveTime_min5	= 20,
	ue_InactiveTime_min6	= 21,
	ue_InactiveTime_min7	= 22,
	ue_InactiveTime_min8	= 23,
	ue_InactiveTime_min9	= 24,
	ue_InactiveTime_min10	= 25,
	ue_InactiveTime_min12	= 26,
	ue_InactiveTime_min14	= 27,
	ue_InactiveTime_min17	= 28,
	ue_InactiveTime_min20	= 29,
	ue_InactiveTime_min24	= 30,
	ue_InactiveTime_min28	= 31,
	ue_InactiveTime_min33	= 32,
	ue_InactiveTime_min38	= 33,
	ue_InactiveTime_min44	= 34,
	ue_InactiveTime_min50	= 35,
	ue_InactiveTime_hr1	= 36,
	ue_InactiveTime_hr1min30	= 37,
	ue_InactiveTime_hr2	= 38,
	ue_InactiveTime_hr2min30	= 39,
	ue_InactiveTime_hr3	= 40,
	ue_InactiveTime_hr3min30	= 41,
	ue_InactiveTime_hr4	= 42,
	ue_InactiveTime_hr5	= 43,
	ue_InactiveTime_hr6	= 44,
	ue_InactiveTime_hr8	= 45,
	ue_InactiveTime_hr10	= 46,
	ue_InactiveTime_hr13	= 47,
	ue_InactiveTime_hr16	= 48,
	ue_InactiveTime_hr20	= 49,
	ue_InactiveTime_day1	= 50,
	ue_InactiveTime_day1hr12	= 51,
	ue_InactiveTime_day2	= 52,
	ue_InactiveTime_day2hr12	= 53,
	ue_InactiveTime_day3	= 54,
	ue_InactiveTime_day4	= 55,
	ue_InactiveTime_day5	= 56,
	ue_InactiveTime_day7	= 57,
	ue_InactiveTime_day10	= 58,
	ue_InactiveTime_day14	= 59,
	ue_InactiveTime_day19	= 60,
	ue_InactiveTime_day24	= 61,
	ue_InactiveTime_day30	= 62,
	ue_InactiveTime_dayMoreThan30	= 63
} e_ue_InactiveTime;

/* RRM-Config */
typedef struct RRM_Config {
	long	*ue_InactiveTime	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRM_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ue_InactiveTime_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_RRM_Config;

#ifdef __cplusplus
}
#endif

#endif	/* _RRM_Config_H_ */
#include <liblte/rrc/asn/asn_internal.h>
