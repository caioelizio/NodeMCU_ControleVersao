
#include <ESP8266httpUpdate.h>
// Intervalo verificação de versão
//const int VCS_CHECK_INT = 60 * 60 * 6; // Sugestão a cada 6 horas
  const int VCS_CHECK_INT = 60;          // 60s apenas para teste

// Controle de Versão
  const char* VCS_SW_VERSION = "1.1";
  const char* VCS_URL = "https://dl.dropboxusercontent.com/s/n2nyf0y2c8hxnz6/vcs.txt"; //formatada //substitui https://dl.dropboxusercontent.com/s... e excluir o final ( ?dl=0 )
/* configuracao do arquivo vcs.txt
// substitui ( ino ) por ( spiffs ) na 2º URL 
{
  "swVersion": "1.0",
  "swMandatory": true
  "swRelease": "2020-04-19T16:00:00Z",
  "swURL": "https://dl.dropboxusercontent.com/s/tljx2ydc9zbehtu/NodeMCU_ControleVersao.ino.bin",
  "fsVersion": "1.0",
  "fsMandatory": false,
  "fsRelease": "2020-04-19T16:00:00Z",
  "fsURL": "https://dl.dropboxusercontent.com/s/v8ac3cmp0sm3dhx/NodeMCU_ControleVersao.spiffs.bin"
}
//*/

// VARIÁVEIS GLOBAIS
// Próxima verificação de versão
time_t vcsNextCheck = 0;

// Dados do arquivo de versão
struct VCS {
  char    swVersion[14];
  bool    swMandatory;
  time_t  swRelease;
  char    swURL[150];
  char    fsVersion[14];
  bool    fsMandatory;
  time_t  fsRelease;
  char    fsURL[150];
};
VCS vcs;
