from flask import Flask, jsonify
app = Flask(__name__)
RESULTS = {"2026001": {"name": "A. Student", "gpa": 8.7}}

@app.route("/")
def index():
    return "<h1>University Result Portal</h1><p>Semester results, Oct 2026</p>"

@app.route("/result/<regno>")
def result(regno):
    return jsonify(RESULTS.get(regno, {"error": "not found"}))

@app.route("/health")
def health():
    return jsonify(status="ok")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, threaded=True)
