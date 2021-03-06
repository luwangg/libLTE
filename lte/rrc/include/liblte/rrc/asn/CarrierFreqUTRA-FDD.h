/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "./asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_CarrierFreqUTRA_FDD_H_
#define	_CarrierFreqUTRA_FDD_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include "liblte/rrc/asn/ARFCN-ValueUTRA.h"
#include "liblte/rrc/asn/CellReselectionPriority.h"
#include "liblte/rrc/asn/ReselectionThreshold.h"
#include <liblte/rrc/asn/NativeInteger.h>
#include "liblte/rrc/asn/ReselectionThresholdQ-r9.h"
#include <liblte/rrc/asn/constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CarrierFreqUTRA-FDD */
typedef struct CarrierFreqUTRA_FDD {
	ARFCN_ValueUTRA_t	 carrierFreq;
	CellReselectionPriority_t	*cellReselectionPriority	/* OPTIONAL */;
	ReselectionThreshold_t	 threshX_High;
	ReselectionThreshold_t	 threshX_Low;
	long	 q_RxLevMin;
	long	 p_MaxUTRA;
	long	 q_QualMin;
	struct CarrierFreqUTRA_FDD__threshX_Q_r9 {
		ReselectionThresholdQ_r9_t	 threshX_HighQ_r9;
		ReselectionThresholdQ_r9_t	 threshX_LowQ_r9;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *threshX_Q_r9;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CarrierFreqUTRA_FDD_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CarrierFreqUTRA_FDD;

#ifdef __cplusplus
}
#endif

#endif	/* _CarrierFreqUTRA_FDD_H_ */
#include <liblte/rrc/asn/asn_internal.h>
