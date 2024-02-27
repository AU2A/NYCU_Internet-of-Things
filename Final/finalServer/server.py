import flask
import os

app = flask.Flask(__name__, static_folder="assets/")


@app.route("/")
def home():
    return flask.render_template("home.html")


@app.route("/test")
def test():
    return flask.render_template("test.html")


@app.route("/upload", methods=["GET"])
def upload():
    field1 = flask.request.args.get("field1")
    field2 = flask.request.args.get("field2")
    field3 = flask.request.args.get("field3")
    field4 = flask.request.args.get("field4")
    field5 = flask.request.args.get("field5")
    field6 = flask.request.args.get("field6")

    print(field1, field2, field3, field4, field5, field6)
    return "ok"


@app.route("/upload-image", methods=["GET", "POST"])
def upload_image():
    if (
        flask.request.method == "POST"
    ):  # if we make a post request to the endpoint, look for the image in the request body
        image_raw_bytes = flask.request.get_data()  # get the whole body

        save_location = os.path.join(
            app.root_path, "assets/test.jpg"
        )  # save to the same folder as the flask app live in

        f = open(
            save_location, "wb"
        )  # wb for write byte data in the file instead of string
        f.write(image_raw_bytes)  # write the bytes from the request body to the file
        f.close()

        print("Image saved")

        return "image saved"

    if (
        flask.request.method == "GET"
    ):  # if we make a get request (for example with a browser) show the image.
        # The browser will cache this image so when you want to actually refresh it, press ctrl-f5
        return flask.render_template("image_show.html")
    return "if you see this, that is bad request method"


app.run(host="", port=5000)
