import serial
import time
import csv

# Configuração da comunicação serial com o Arduino
arduino = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)

def buscar_informacoes_cliente(id_digital):
    with open('clientes.csv', newline='', encoding='utf-8') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if row['id'] == str(id_digital):
                nome = row['nome']
                data_ultima_compra = row['data_ultima_compra']
                valor_total_compras = row['valor_total_compras']
                return {
                    "nome": nome,
                    "data_ultima_compra": data_ultima_compra,
                    "valor_total_compras": valor_total_compras
                }
    return None

def acender_luz_verde():
    arduino.write(b'1')
    resposta = arduino.readline().decode().strip()
    print(resposta)

def desligar_luz_verde_e_acender_vermelho():
    arduino.write(b'0')
    resposta = arduino.readline().decode().strip()
    print(resposta)

def on_button_click():
    print("Botão 'digital' clicado, preparando leitura de digital...")
    acender_luz_verde()

    while True:
        if arduino.in_waiting > 0:
            leitura = arduino.readline().decode().strip()
            if leitura.startswith("ID:"):
                id_digital = int(leitura.split("ID:")[1].strip())
                print(f"ID da digital: {id_digital}")
                
                # Buscar informações do cliente
                cliente_info = buscar_informacoes_cliente(id_digital)
                if cliente_info:
                    print(f"Nome: {cliente_info['nome']}")
                    print(f"Data da última compra: {cliente_info['data_ultima_compra']}")
                    print(f"Valor total de compras: R${cliente_info['valor_total_compras']}")
                else:
                    print("Cliente não encontrado.")

                break
    
    desligar_luz_verde_e_acender_vermelho()

# Simulação de clique no botão
on_button_click()

arduino.close()
