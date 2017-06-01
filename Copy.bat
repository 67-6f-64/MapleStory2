//명령어		옵션		복사할경로		복사 받을 경로

xcopy /y	.\Engine\Headers\*.h					.\Reference\Headers\
xcopy /y	.\Engine\Export\*.*						.\Reference\Headers\

xcopy /y	.\Engine\System\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\System\bin\System.lib			.\Reference\Library\
xcopy /y	.\Engine\System\bin\System.dll			.\Client\bin\

xcopy /y	.\Engine\Utility\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Utility\bin\Utility.lib		.\Reference\Library\
xcopy /y	.\Engine\Utility\bin\Utility.dll		.\Client\bin\

xcopy /y	.\Engine\Resources\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Resources\bin\Resources.lib	.\Reference\Library\
xcopy /y	.\Engine\Resources\bin\Resources.dll	.\Client\bin\