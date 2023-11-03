set curr_dir=%~dp0
set exec_dir=%curr_dir%..\..\Engine\External\Vulkan\Bin

call %exec_dir%\glslc.exe %curr_dir%\BasicShader.vert -o %curr_dir%\BasicShader.vert.spv
call %exec_dir%\glslc.exe %curr_dir%\BasicShader.frag -o %curr_dir%\BasicShader.frag.spv

echo Shaders compilation completed.
pause
