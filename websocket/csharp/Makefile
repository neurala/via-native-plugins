CSC := csc
CSCFLAGS := /langversion:latest /nologo /optimize

BINARIES := Binaries
SOURCES := Sources

test : $(BINARIES)/Test.exe
websocket : $(BINARIES)/Neurala.VIA.WebSocket.dll

$(BINARIES)/Test.exe : $(BINARIES)/Neurala.VIA.WebSocket.dll $(SOURCES)/Test.cs
	$(CSC) $(CSCFLAGS) /out:$@ /t:exe /r:$(BINARIES)/Neurala.VIA.WebSocket.dll $(filter %.cs,$^)

$(BINARIES)/Neurala.VIA.WebSocket.dll : $(SOURCES)/Server.cs
	$(CSC) $(CSCFLAGS) /out:$@ /t:library /r:$(BINARIES)/Newtonsoft.Json.dll /r:$(BINARIES)/WebSocketSharp.dll $(filter %.cs,$^)
