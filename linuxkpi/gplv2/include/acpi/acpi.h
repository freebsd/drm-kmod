#include <sys/param.h>
#if __FreeBSD_version < 1300118
#ifndef _ACPI_ACPI_H_
#define _ACPI_ACPI_H_

typedef ACPI_HANDLE		acpi_handle;
typedef ACPI_OBJECT		acpi_object;
typedef ACPI_OBJECT_HANDLER	acpi_object_handler;
typedef ACPI_OBJECT_TYPE	acpi_object_type;
typedef ACPI_STATUS		acpi_status;
typedef ACPI_STRING		acpi_string;
typedef ACPI_SIZE		acpi_size;
typedef ACPI_WALK_CALLBACK	acpi_walk_callback;

#define	acpi_evaluate_object	AcpiEvaluateObject
#define	acpi_format_exception	AcpiFormatException
#define	acpi_get_handle		AcpiGetHandle
#define	acpi_get_data		AcpiGetData
#define	acpi_get_name		AcpiGetName
#define	acpi_get_table		AcpiGetTable

#endif /* _ACPI_ACPI_H_ */
#else
#include_next <acpi/acpi.h>
#endif
