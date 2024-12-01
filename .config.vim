" this config is for project

" name     : the task name
" mode     : (new version add)the mode of task,quickfix,term,workflow,shell
"            workflow first exec task [begin],and if first exec is ok,exec task [end]
"            use workflow mode,first exec must be quickfix
"            workflow mode option is only with:begin,next,name,key
"            shell model only run in backgroud and show nothing which is always success
" command  : the command to run
" path     : path can be termtask#Term_get_dir() stand for git,expand("%:p:h") for current dir
"            expand("%") for current buffer all path,expand("%:t") for current buffer name
"            use . can connect str
" close    : term and quickfix after command status
"            0 noclose
"            1 close
"            2 if command is not ok,will stay open,or close,
"            3 do not open first,after finish,open it(invalid for nvim)
" type     : pos of task tab or vsplit or split
" key      : key can bound map to make task run
" script   : pre,end script is vimscript,use | to divide
"            can define function,and make end_script call func to do it(recommand way)
"            can use g:asyncrun_code==0 to judge task if success
let s:root=termtask#Term_get_dir().'/'
let s:pwd=expand('%:p:h')
let g:Term_project_task=[
			\{
			\'name'       : 'build',
			\'command'    : './build.sh true release',
			\'mode'       : 'quickfix',
			\'path'       : s:root.'/src',
			\'close'      : 2,
			\'type'       : 'split',
			\'key'        : '\1',
			\'pre_script' : '',
			\'end_script' : '',
			\},
			\{
			\'name'       : 'run',
			\'command'    : './bin/test_bin',
			\'mode'       : 'term',
			\'path'       : s:root.'/src',
			\'close'      : 0,
			\'type'       : 'vsplit',
			\'key'        : '\2',
			\},
			\{
			\'name'       : 'workflow',
			\'mode'       : 'workflow',
			\'begin'      : 'build',
			\'next'       : 'run',
			\'key'        : '\3',
			\},
			\{
			\'name'       : 'debug',
			\'command'    : './build.sh ON Debug',
			\'mode'       : 'quickfix',
			\'path'       : s:root.'/src',
			\'close'      : 1,
			\'type'       : 'split',
			\'key'        : '\4',
			\'end_script' : 'cclose|packadd termdebug|Termdebug '. s:root.'/src/bin/test_bin',
			\},
			\{
			\'name'       : 'coc',
			\'command'    : 'cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1;cp ./compile_commands.json ..',
			\'mode'       : 'quickfix',
			\'path'       : s:root.'/src/build',
			\'close'      : 2,
			\'end_script' : 'CocRestart',
			\'type'       : 'split',
			\'key'        : '\5',
			\},
			\{
			\'name'       : 'release',
			\'command'    : './release.sh',
			\'mode'       : 'quickfix',
			\'path'       : s:root.'/src',
			\'close'      : 2,
			\'type'       : 'split',
			\'key'        : '\6',
			\'pre_script' : '',
			\'end_script' : '',
			\},
			\]

