# define the name of the installer
SetCompressor /FINAL /SOLID lzma 
outfile "Cape2Fluent.exe"
 
# define the directory to install to, the desktop in this case as specified  
# by the predefined $DESKTOP variable
installDir "c:\Cape2Fluent"
Page components
Page directory
Page instfiles
# default section
Section "VC Redistributable 2008"
	ClearErrors
	ExecWait "$EXEDIR\vcredist_x86.exe" $0
	DetailPrint "some program returned $0"
	IfErrors 0 +2
		messageBox MB_OK "Installation of VC Redist failed. Check if the file vcredist_x86.exe is present in installator directory or install it manually."
	
SectionEnd

Section "Cape2Fluent"

	# g³ówny katalog
	setOutPath $INSTDIR
	file Cape2Fluent\*.*
		
	setOutPath $INSTDIR
	
	writeUninstaller $INSTDIR\uninstall.exe
	
	WriteRegExpandStr HKCU "Software\Cape2Fluent" "InstallDir" "$INSTDIR"

	# create a shortcut named "new shortcut" in the start menu programs directory
    # presently, the new shortcut doesn't call anything (the second field is blank)
	CreateDirectory "$SMPROGRAMS\Cape2Fluent"
    createShortCut "$SMPROGRAMS\Cape2Fluent\Params.lnk" "$INSTDIR\params.txt"
	createShortCut "$SMPROGRAMS\ADDIP\uninstall.lnk" "$INSTDIR\uninstall.exe"
 SectionEnd

# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
Section "Uninstall"
 
# Always delete uninstaller first
	delete "$INSTDIR\uninstall.exe"

    delete "$SMPROGRAMS\Cape2Fluent\Params.lnk"
	delete "$SMPROGRAMS\Cape2Fluent\uninstall.lnk"
	RMDir "$SMPROGRAMS\Cape2Fluent"
	DeleteRegKey HKCU "Software\Cape2Fluent"

# now delete installed file
	messageBox MB_OK "Please, delete Cape2Fluent installation directory manually"
SectionEnd