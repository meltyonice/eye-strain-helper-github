use std::str::FromStr;

use tiny_http::{HeaderField, Response, Server};
use ipckit::SharedMemory;
use ascii::{AsciiString};

fn main() -> ipckit::Result<()> {
    let server = Server::http("0.0.0.0:7289").unwrap();
    let mut shm = SharedMemory::open("eshs2rs")?;
    let _ = shm.write(2, b"11");
    for request in server.incoming_requests() {
        if request.method() == &tiny_http::Method::NonStandard(AsciiString::from_ascii("HEARTBEAT").unwrap()) { 
            let _ = shm.write(6, b"HEARTBEAT");
            let _ = request.respond(Response::from_string("200"));
        } else if request.method() == &tiny_http::Method::NonStandard(AsciiString::from_ascii("PING").unwrap())  { //Settle down, yanderedev
            let _ = request.respond(Response::from_string("Pong!"));
        }
    }
    Ok(())
}
