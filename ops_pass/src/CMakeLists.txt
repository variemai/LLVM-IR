add_llvm_pass_plugin(OpsPass
    # List your source files here.
   opspass.cpp
   #profilingfunctions.cpp
   #PLUGIN_TOOL
   #opt
)

#llvm_map_components_to_libnames(llvm_libs Core Support)
#target_link_libraries(OpsPass PRIVATE ${llvm_libs})
