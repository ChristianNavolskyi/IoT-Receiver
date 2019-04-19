# invoke SourceDir generated makefile for rfExamples.pem3
rfExamples.pem3: .libraries,rfExamples.pem3
.libraries,rfExamples.pem3: package/cfg/rfExamples_pem3.xdl
	$(MAKE) -f C:\Users\user\Documents\GitHub\IoT-Receiver/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\user\Documents\GitHub\IoT-Receiver/src/makefile.libs clean

