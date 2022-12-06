## Selecione o Idioma: | Português | [Inglês](./README_en.md) | [Espanhol](./README_es.md) |

# Adicional_de_Alarme_Auto

Projeto de um Adicional para alarme automotivo, que lê a quantidade de pulsos do Alarme e toma uma decisão.

## Objetivo e Motivação do Projeto.

Este projeto foi criado para ser utilizado com o rastreador chinês TK-303 ou qualquer outro Alarme que não possua o acionamento das travas eletricas ou do vidro elétrico de forma nativa.


# Recursos

- 1 pulso  - Fecha as Travas eléticas, Pisca a Seta e sobe o Vidro
- 2 pulsos - Abre a Trava elétrica e Pisca a Seta duas vezes
- 3 pulsos - Não faz nada.
- Identifica se o Farol esta ligado e sem o Pós chave emite som de alerta

# BOM


# Schematico

![plot](./images/schematico1.jpg)


# Visão 3D da PCI

![plot](./images/TopBotton3dLayer.jpg)


# Layout das Trilhas da PCI

![plot](./images/topbot-layout.jpg)


# Revisões

* v1.0.2 - 
Modificação do Layout para a inclusão dos diodos schottky D2, D3, circuito de acionamento da buzzer, modificação do circuito de acionamento do Rele. 


* v1.0.1 - 
Placa com correção nas medidas do Tamanho para que possam caber na Caixa Patola CP-021


* v1.0.0 - 
Placa inicial feita na JLCPCB com alguns erros de projetos pegos na produção
