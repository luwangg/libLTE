/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "./asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_UEInformationResponse_v930_IEs_H_
#define	_UEInformationResponse_v930_IEs_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include <liblte/rrc/asn/OCTET_STRING.h>
#include <liblte/rrc/asn/constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct UEInformationResponse_v1020_IEs;

/* UEInformationResponse-v930-IEs */
typedef struct UEInformationResponse_v930_IEs {
	OCTET_STRING_t	*lateNonCriticalExtension	/* OPTIONAL */;
	struct UEInformationResponse_v1020_IEs	*nonCriticalExtension	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UEInformationResponse_v930_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UEInformationResponse_v930_IEs;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "liblte/rrc/asn/UEInformationResponse-v1020-IEs.h"

#endif	/* _UEInformationResponse_v930_IEs_H_ */
#include <liblte/rrc/asn/asn_internal.h>
