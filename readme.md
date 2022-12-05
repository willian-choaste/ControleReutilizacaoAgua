# Protótipo sistema de controle e reutilização da água da chuva

Este projeto IOT (Internet das Coisas) tem como o objetivo realizar a gestão da água da chuva, visando o melhor aproveitamento para água de acordo com sua qualidade, contribuindo assim com sexto Objetivo de Desenvolvimento Sustentável da ONU, com foco na gestão sustentável da água.
![ODS 6 - Água potável e saneamento](images/ODS%206%20-%20%C3%81gua%20pot%C3%A1vel%20e%20saneamento.png)


## Funcionalidades do sistema
- Monitorar nível de água na cisterna;
- Monitorar nível de água na caixa;
- Monitorar nível de  TDS (Total de Sólidos Dissolvidos) na água;
- Informar dados coletados através do monitoramento de forma visual;
- Funcionar de modo automático, tendo opção para controle manual via aplicativo.

## Diagrama de caso de uso
![Diagrama de caso de uso](images/Caso%20de%20Uso%20-%20Sistema%20de%20Controle%20e%20Monitoramento%20%C3%81gua%20da%20Chuva.png)

## Diagrama de atividades
![Diagrama de atividades](images/Diagrama%20de%20Atividades%20-%20Sistema%20de%20Controle%20e%20Monitoramento%20%C3%81gua%20da%20Chuva.png)

## Arquitetura do projeto
O projeto foi desenvolvido utilizando a [IDE PlatformIO](https://platformio.org/), pois esta atendia a necessidade de realizar a programação visando a utilização da placa ESP32.

Abaixo esta a organização da estrutura do projeto.
![Arquitetura projeto](images/Arquitetura%20projeto.png)

O projeto foi desenvolvido em linguagem c++. 
![Logo C++](images/logo%20c%2B%2B.png)

Foi utilizada a placa ESP32 como harware de execução do software.
![Pinout ESP32](images/pinout%20esp32%20nodemcu.png)

Os aplicativos móvel e web fooram desenvolvidos utilizando a [Blynk Iot plataform](https://blynk.io/), que é voltada há aplicações em Internet das Coisas (Internet of Things - IoT), ela permite o desenvolvimento de interfaces para controle e monitoramento, coleta de dados de sensores, criação de visualizações gráficas e armazenamento de dados de forma automática na nuvem BlynkCloud. Também é possível através da plataforma o enviar notificações ao aplicativo.

![Arquitetura Blynk](images/Arquitetura%20de%20funcionamento%20do%20Blynk.png)

## Desenvolvimento do projeto

O projeto foi realizado após a definição do hardware que buscou atender as demandas do requisitos, com base no sistema de captação de água da chuva da Cedae (Companhia Estadual de Águas e Esgotos do Rio de Janeiro).
Em destaque nas caixas vermelhas, estão as modificações definadas para a realização do projeto.
![Sistema Captação de água](images/sistema%20reutiliza%C3%A7%C3%A3o%20%C3%A1gua%20da%20chuva%20-%20cedae%20-%20com%20modifica%C3%A7%C3%B5es%20de%20uso.png)

Foram adicionados como meios de controle para esse sistema, sensores de monitoramento de nível da água tanto no  reservatório, quanto na caixa da água destinada a reutilização, bombas da água para realizar o transporte entre o reservatorio e a caixa, e o reservatório e a rede de água/esgoto. Adicionado um sensor TDS (Total de Sólidos Dissolvidos) para monitoramento da qualidade da água na caixa de reutilização e por fim eletroválvulas responsavéis por liberar a água para uso de acordo com a leitura apresentada pelo sensor TDS.

Para o desenvolvimento back-end foi utilizado a programação modular que consiste em uma técnica de design de software em que as soluções digitais são formadas por módulos independentes e intercambiáveis.
Como por exemplo as funções abaixo:
- Funções funcionamento bombas d'água
![Funções funcionamento bombas](images/Fun%C3%A7%C3%B5es%20funcionamentos%20bombas.png)

Para a validação das funções do sistema foi utilizado o [Wokwi](https://wokwi.com/), um simulador para projetos IoT.
![Testes simulador](images/testes%20simulador.png)


Para o desenvolvimento front-end foi utilizado a Blynk Iot plataform, que já foi comentada acima.

- Interface aplicativo web
![Interfac aplicativo web](images/Inteface%20app%20web.png)

- Interface aplicativo móvel
![Interfac aplicativo móvel](images/interface%20app%20movel.png)

Por fim para a validação do envio e recebimento de dados através dos apps web e mobile, foram realizados diversos testes com o protótipo em operação.

- Protótipo utilizado para os testes
![Interfac aplicativo móvel](images/prot%C3%B3tipo%20de%20teste.jpeg)