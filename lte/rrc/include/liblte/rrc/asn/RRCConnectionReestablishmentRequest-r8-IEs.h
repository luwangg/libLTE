/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "./asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_RRCConnectionReestablishmentRequest_r8_IEs_H_
#define	_RRCConnectionReestablishmentRequest_r8_IEs_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include "liblte/rrc/asn/ReestabUE-Identity.h"
#include "liblte/rrc/asn/ReestablishmentCause.h"
#include <liblte/rrc/asn/BIT_STRING.h>
#include <liblte/rrc/asn/constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RRCConnectionReestablishmentRequest-r8-IEs */
typedef struct RRCConnectionReestablishmentRequest_r8_IEs {
	ReestabUE_Identity_t	 ue_Identity;
	ReestablishmentCause_t	 reestablishmentCause;
	BIT_STRING_t	 spare;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCConnectionReestablishmentRequest_r8_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCConnectionReestablishmentRequest_r8_IEs;

#ifdef __cplusplus
}
#endif

#endif	/* _RRCConnectionReestablishmentRequest_r8_IEs_H_ */
#include <liblte/rrc/asn/asn_internal.h>
