from flask import Flask, request, jsonify

app = Flask(__name__)

temp_list = []

@app.route('/')
def hello():
    return 'HALOO, SELAMAT DATANG DI SERVER API'

@app.route('/api/data', methods=["POST", "GET"])
def sensor():
    if request.method == 'POST':
        data = request.get_json()

        # Memastikan semua field yang diperlukan ada dalam data
        if not all(key in data for key in ('temperature', 'humidity', 'timestamp')):
            return jsonify({'error': 'Missing required fields'}), 400
        
        # Mengambil nilai temperature, humidity, dan timestamp dari data
        temperature = data["temperature"]
        humidity = data["humidity"]
        timestamp = data["timestamp"]

        # Menambahkan data ke temp_list
        temp_list.append({
            'temperature': temperature,
            'humidity': humidity,
            'timestamp': timestamp
        })

        # Membuat respons sukses
        response_data = {
            'message': 'Data saved successfully'
       
}
        
    else:
        # Menyiapkan respons dengan daftar semua data yang tersimpan
        response_data = {
            'temperature_list': temp_list
        }

    # Mengembalikan respons dalam format JSON
    return jsonify(response_data)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
