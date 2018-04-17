rem for /r .\ %%a in (Debug) do @if exist %%a rd /s /q %%a
for /r .\ %%a in (ipch) do @if exist %%a rd /s /q %%a

del /S  *.ncb
del /S  *.opt
del /S  *.plg
del /S  *.positions
del /S  *.pdb
del /S  *.idb
del /S  *.ilk
del /S  *.obj
del /S  *.pch
del /S  *.sdf
del /S  *.ipch
del /S  *.sbr
del /S  *.bsc
del /S  *.exp