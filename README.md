Câmera de Segurança com controle por wifi - ESP32 CAM e Módulo "Pan e Tilt"

Implementação de uma câmera de segurança remota acessível via navegador web, utilizando um módulo ESP32-CAM (modelo AI Thinker).

    Streaming de Vídeo: Transmissão MJPEG via HTTP.

    Controle de Movimento: Interface para controlar servos nos eixos X (Base) e Y (Câmera).

    Ajustes de Imagem: Alteração dinâmica de resolução (Framesize) e qualidade JPEG.

    Controle de Iluminação: Ajuste da intensidade do LED Flash embutido via PWM.

    Conectividade: Tenta conectar a uma rede Wi-Fi pré-configurada; caso falhe, cria um Ponto de Acesso próprio.

Pinagem

    Servo da Base (Pan): GPIO 2

    Servo da Câmera (Tilt): GPIO 14

    LED Flash Interno: GPIO 4

    Câmera: Interface padrão OV2640 (AI Thinker)

Configuração e Instalação

    Credenciais de Rede: Abra o arquivo cameraRemota.ino e edite as variáveis globais no início do código com o nome e senha da sua rede Wi-Fi:

        const char* ssid = "...";

        const char* password = "...";

    Upload: Utilize a IDE do Arduino com a placa selecionada como "AI Thinker ESP32-CAM". Certifique-se de ter as bibliotecas do ESP32 instaladas corretamente.

    Acesso: Após o upload, abra o Monitor Serial (Baud Rate 115200). Reinicie a placa. O IP de acesso será exibido no console. Digite este IP no navegador para acessar a interface de controle.

        Caso a conexão Wi-Fi falhe, o dispositivo criará uma rede chamada "ESP32-CAM" com senha "12345678". O IP padrão neste modo geralmente é 192.168.4.1.

Notas

    Estabilidade de Energia: O código desativa o detector de Brownout (RTC_CNTL_BROWN_OUT_REG = 0). Isso evita reinicializações automáticas quando a tensão cai, o que é comum ao acionar servos e Wi-Fi simultaneamente. Recomendação: Utilize uma fonte de alimentação externa robusta (5V, 2A ou mais) para os servos. Não alimente os servos diretamente pelo pino de 3.3V ou 5V do ESP32-CAM, pois o pico de corrente pode travar o microcontrolador. Lembre-se de conectar o GND da fonte externa ao GND do ESP32.

    Gerenciamento de Memória: O código verifica a presença de PSRAM. Se disponível, a qualidade e o buffer de frames são aumentados. Caso contrário, a qualidade é reduzida para economizar memória.

    Inicialização dos Servos: Os servos são inicializados com PWM de 50Hz e resolução de 16-bit. Certifique-se de que os servos utilizados suportam essa frequência para evitar superaquecimento ou "jitter" (tremedeira).

    Dissipação de Calor: O streaming de vídeo contínuo gera calor significativo no chip ESP32. Evite instalar o módulo em caixas hermeticamente fechadas sem ventilação para garantir a longevidade do componente.
