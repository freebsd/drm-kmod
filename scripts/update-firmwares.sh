#!/bin/sh

set -e

usage()
{
	cat << EOF
	Usage: `basename $0` <linux-firmware-dir> <freebsd-firmware-dir>
ex: `basename $0` ../linux-firmware ../drm-kmod-firmware
EOF

	exit 1
}

if [ $# -ne 2 ]; then
	usage
fi

LINUX_FW_DIR=$(realpath "$1")
shift
FREEBSD_FW_DIR=$(realpath "$1")
shift

if [ ! -d ${LINUX_FW_DIR} ]; then
	echo "linux-firmware directory not found: ${LINUX_FW_DIR}"
	exit 1
fi

if [ ! -d ${FREEBSD_FW_DIR} ]; then
	echo "linux-firmware directory not found: ${FREEBSD_FW_DIR}"
	exit 1
fi

scriptdir=$(dirname "$0")
drm_dir=$scriptdir/..
fw_list=$scriptdir/fw-list.txt

# --------------------------------------------------------------------
# List firmwares referenced in the DRM drivers source code.
# --------------------------------------------------------------------

: > "$fw_list"

git -C "$drm_dir" grep -P '^\s*fw_def' | \
awk '
/i915\/.*guc_maj/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\).*/, "", fw);
  sub(/, */, "_guc_", fw);
  sub(/,.*/, "", fw);
  print "i915/" fw ".bin";
  next;
}
/i915\/.*guc_mmp/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\).*/, "", fw);
  sub(/, */, "_guc_", fw);
  gsub(/, */, ".", fw);
  print "i915/" fw ".bin";
  next;
}
/i915\/.*huc_raw/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\).*/, "", fw);
  sub(/, */, "_huc", fw);
  sub(/,.*/, "", fw);
  print "i915/" fw ".bin";
  next;
}
/i915\/.*huc_gsc/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\).*/, "", fw);
  sub(/, */, "_huc_gsc", fw);
  sub(/,.*/, "", fw);
  print "i915/" fw ".bin";
  next;
}
/i915\/.*huc_mmp/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\).*/, "", fw);
  sub(/, */, "_huc_", fw);
  gsub(/, */, ".", fw);
  print "i915/" fw ".bin";
  next;
}
/i915\/.*gsc_def/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\).*/, "", fw);
  sub(/, */, "_gsc", fw);
  sub(/,.*/, "", fw);
  print "i915/" fw ".bin";
  next;
}
' >> "$fw_list"

git -C "$drm_dir" grep -P 'DMC_(LEGACY_|)PATH\(' | \
awk '
/DMC_PATH\(.*\)$/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/\)/, "_dmc", fw);
  print "i915/" fw ".bin";
  next;
}
/DMC_LEGACY_PATH\(.*\)$/ {
  fw = $0;
  sub(/.*\(/, "", fw);
  sub(/, */, "_dmc_ver", fw);
  sub(/, */, "_", fw);
  sub(/\)/, "", fw);
  print "i915/" fw ".bin";
  next;
}
' >> "$fw_list"

git -C "$drm_dir" grep -P '\.bin"' | \
awk '
/:(MODULE_FIRMWARE\(|#define.*)"/ {
  fw = $0;
  sub(/[^"]+"/, "", fw);
  sub(/".*/, "", fw);
  print fw;
  next;
}
' >> "$fw_list"

# --------------------------------------------------------------------
# Copy the listed firmwares to the FreeBSD firmware directory.
# --------------------------------------------------------------------

for file in $(cat "$fw_list"); do
	src="$LINUX_FW_DIR/$file"

	dst="${file%%/*}kmsfw-files/${file##*/}"
	dst="$FREEBSD_FW_DIR/$dst"

	! test -f "$src" || cp "$src" "$dst"
done

rm -f "$fw_list"

# --------------------------------------------------------------------
# Generate Makefiles.
# --------------------------------------------------------------------

for dir in "i915kmsfw" "amdgpukmsfw" "radeonkmsfw"; do
	"$FREEBSD_FW_DIR/$dir/gen-makefiles"
done
