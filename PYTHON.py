import socket
import threading

# --- CONFIGURACIÓN DE RED ---
PC_IP = "192.168.200.103"      # IP de tu ordenador
STM32_IP = "192.168.200.99"    # IP del destino (STM32)
LISTENING_PORT = 35000         # Puerto donde el PC escucha (STM32 envía aquí)
SENDER_PORT = 45000            # Puerto donde el PC envía (STM32 escucha aquí)
SYNC_BYTE = b'\xAC'            # Byte de cabecera obligatorio

# Crear el socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((PC_IP, LISTENING_PORT))

# --- FUNCIÓN DE RECEPCIÓN (Se ejecuta en segundo plano) ---
def listen_thread():
    while True:
        try:
            # recvfrom se queda esperando hasta que llegan datos (no consume CPU)
            data, addr = sock.recvfrom(1024) 
            hex_data = data.hex(' ').upper() # Formatea a ej: "AC 01 FF 2B"
            
            # Verificar si empieza por el byte correcto
            if data[0] == 0xAC:
                print(f"\n[<-- RX STM32] {hex_data}\n> ", end="")
            else:
                print(f"\n[<-- RX STM32] (Aviso: No empieza por AE) {hex_data}\n> ", end="")
        except Exception:
            break # Sale del bucle si el socket se cierra

# Iniciar el hilo de recepción
t = threading.Thread(target=listen_thread, daemon=True)
t.start()

print(f"[*] Simulador MMIIB iniciado en {PC_IP}:{LISTENING_PORT}")
print(f"[*] Destino configurado a {STM32_IP}:{SENDER_PORT}")
print("[*] Escribe los bytes en hexadecimal separados por espacios (ej: 01 FF 4A).")
print("[*] NOTA: El byte 'AC' se añadirá automáticamente al principio si no lo pones.\n")

# --- BUCLE PRINCIPAL DE ENVÍO ---
try:
    while True:
        user_input = input("> ").strip()
        if not user_input:
            continue
        
        # Limpiar espacios para procesar la cadena hexadecimal
        clean_input = user_input.replace(" ", "")
        
        try:
            # Convertir el texto hexadecimal a bytes crudos
            payload = bytes.fromhex(clean_input)
            
            # Asegurar que la trama empiece por 0xAE
            if not payload.startswith(SYNC_BYTE):
                payload = SYNC_BYTE + payload
                
            # Enviar los bytes crudos por UDP
            sock.sendto(payload, (STM32_IP, SENDER_PORT))
            print(f"[--> TX MMIIB] Enviado: {payload.hex(' ').upper()}")
            
        except ValueError:
            print("[!] Error: Formato no válido. Usa solo caracteres hexadecimales (0-9, A-F).")

except KeyboardInterrupt:
    print("\n[*] Cerrando simulador...")
    sock.close()
