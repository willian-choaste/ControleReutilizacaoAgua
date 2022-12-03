# Protótipo sistema de controle e reutilização da água da chuva

Este projeto IOT (Internet das Coisas) tem como o objetivo realizar a gestão da água da chuva, visando o melhor aproveitamento para água de acordo com sua qualidade.

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

O projeto foi desenvolvido em linguagem c++. 

![Arquitetura projeto](images/Arquitetura%20projeto.png)

Foi utilizada a placa ESP32 como harware de execução do software.
![Pinout ESP32](images/Pinout%20ESP32.png)

## Desenvolvimento do projeto
O projeto foi desenvolvido atacando cada funcionalidade de forma modular, deste modo obtive um código mais eficiente, pois assim tratamos com mais foco cada funcionalidade.

- Função nível de água cisterna
![Função nível de água cisterna](images/Fun%C3%A7%C3%A3o%20nivel%20de%20%C3%A1gua%20cisterna.png)

- Funções funcionamento bombas d'água
![Funções funcionamento bombas](images/Fun%C3%A7%C3%B5es%20funcionamentos%20bombas.png)