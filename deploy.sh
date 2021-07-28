# Run this script by root account
SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname "$SCRIPT")
# i=1 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=2 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=3 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=4 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=5 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=6 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=7 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh
# i=8 && ssh node$i bash ${SCRIPT_DIR}/root_cp_run.sh

i=2 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
i=3 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
i=4 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
i=5 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
i=6 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
# i=7 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
i=8 && ssh node$i bash ${SCRIPT_DIR}/root_setup_service.sh
