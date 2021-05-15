
String softwareStr();
void handleHome();
void handleVCS();
void handleStream(const String f, const String mime);

String softwareStr() {
  // Retorna nome do software
  return String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);
}

void handleHome() {
  // Home
  File file = SPIFFS.open(F("/controle_versao.html"), "r");
  if (file && !file.isDirectory()) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    // Dispositivo
    s.replace(F("#sw#")             , softwareStr());
//    s.replace(F("#hw#")             , platform());
    // Versão atual
    s.replace(F("#curSWVersion#")   , swCurrentVersion());
    s.replace(F("#curSWRelease#")   , dateTimeStr(swCurrentBuild()));
    s.replace(F("#curFSVersion#")   , fsCurrentVersion());
    s.replace(F("#curFSRelease#")   , dateTimeStr(fsCurrentBuild()));
    // Versão nova
    s.replace(F("#newSWVersion#")   , vcs.swVersion);
    s.replace(F("#newSWRelease#")   , dateTimeStr(vcs.swRelease, TIMEZONE));
    s.replace(F("#newSWMandatory#") , vcs.swMandatory ? "Sim" : "Não");
    s.replace(F("#newSWURL#")       , vcs.swURL);
    s.replace(F("#newFSVersion#")   , vcs.fsVersion);
    s.replace(F("#newFSRelease#")   , dateTimeStr(vcs.fsRelease, TIMEZONE));
    s.replace(F("#newFSMandatory#") , vcs.fsMandatory ? "Sim" : "Não");
    s.replace(F("#newFSURL#")       , vcs.fsURL);
    // Status
    String st = "";
    if (swCurrentVersion() != vcs.swVersion && fsCurrentVersion() != vcs.fsVersion) {
      // Atualizar SW e FS
      st = F("Atualizar Software e Interface");
    } else if (swCurrentVersion() != vcs.swVersion) {
      // Atualizar SW
      st = F("Atualizar Software");
    } else if (fsCurrentVersion() != vcs.fsVersion) {
      // Atualizar FS
      st = F("Atualizar Interface");
    }
    if (st == "") {
      // Nenuma atualização necessária
      st = F("<span class=\"text-success\">Versão do Software e Interface estão atualizadas</span>");
    } else {
      st += F(" <a href=\"vcs\"><b> Clique para Atualizar</b></a>");
    }
    s.replace(F("#status#") , st);

    // Envia dados
    server.send(200, F("text/html"), s);
    log("Home - Cliente: " + ipStr(server.client().remoteIP()) +
        (server.uri() != "/" ? " [" + server.uri() + "]" : ""));
  } else {
    server.send(500, F("text/plain"), F("Home - ERROR 500"));
    log(F("Home - ERRO lendo arquivo"));
  }
}
void handleVCS() {
  // Manual Version update
  File file = SPIFFS.open(F("/atualizando_versao.html"), "r");
  if (file && !file.isDirectory()) {
    file.setTimeout(100);
    String s = file.readString();
    file.close();

    // Atualiza conteúdo dinâmico
    // Dispositivo
    s.replace(F("#sw#")             , softwareStr());
//    s.replace(F("#hw#")             , platform());

    // Status
    byte t = 1;
    String st = "";
    if (swCurrentVersion() != vcs.swVersion) {
      // Atualizar SW
      st = "Software: " + swCurrentVersion() + " &rarr; " + vcs.swVersion + "<br>";
      // Tempo de espera
      #ifdef ESP8266
        t += 90;
      #else
        t += 70;
      #endif
    }
    if (fsCurrentVersion() != vcs.fsVersion) {
      // Atualizar FS
      st += "Interface: " + fsCurrentVersion() + " &rarr; " + vcs.fsVersion + "<br>";
      // Tempo de espera
      #ifdef ESP8266
        t += 90;
      #else
        t += 70;
      #endif
    }
    s.replace(F("#status#")         , st);

    // Tempo de espera
    s.replace(F("#time#")           , String(t));

    // Envia dados
    server.send(200, F("text/html"), s);
    log("VCS - Cliente: " + ipStr(server.client().remoteIP()));
    hold(1000);

    // Efetua atualização
    vcsUpdate();
  } else {
    server.send(500, F("text/plain"), F("VCS - ERROR 500"));
    log(F("VCS - ERRO lendo arquivo"));
  }
}
void handleStream(const String f, const String mime) {
  // Processa requisição de arquivo
  File file = SPIFFS.open("/" + f, "r");
  if (file && !file.isDirectory()) {
    // Define cache para 2 dias
    server.sendHeader(F("Cache-Control"), F("public, max-age=172800"));
    server.streamFile(file, mime);
    file.close();
    log(f + " - Cliente: " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, F("text/plain"), F("Stream - ERROR 500"));
    log(F("Stream - ERRO lendo arquivo"));
  }
}
