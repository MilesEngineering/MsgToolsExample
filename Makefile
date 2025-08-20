include embedded/ucplatform/mk/include.mk
# codegen.mk will run the code generator for these specific languages
CODEGEN_LANGUAGES := codegen_check codegen_c codegen_cpp codegen_dart codegen_html codegen_java codegen_js codegen_kotlin codegen_matlab codegen_python codegen_simulink codegen_simulink_structs codegen_swift codegen_cosmos
include $(MK_DIR)/codegen.mk


SUBDIRS := embedded

# subdir.mk will run Make for all dirs in the SUBDIRS variable.
include $(MK_DIR)/subdir.mk
