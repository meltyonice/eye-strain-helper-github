use std::str::FromStr;

use tiny_http::{Header, HeaderField, Response, Server};
use ipckit::SharedMemory;

fn main() -> ipckit::Result<()> {
    let server = Server::http("0.0.0.0:7289").unwrap();
    let mut shm = SharedMemory::open("eshs2rs")?;
    let _ = shm.write(2, b"1111");
    for request in server.incoming_requests() {
        //dbg!(request.method(),request.url(),request.headers());

        if request.url() == "/heartbeat" {
            for h in request.headers() {   
                if h.field == HeaderField::from_str("User-Agent").unwrap() {
                    let _ = shm.write(6, b"HEARTBEAT");
                    let _ = request.respond(Response::from_string("200"));
                    break;
                }
            }

            
        }

        //let response = Response::from_string("hello world");
        //request.respond(response);
    }
    Ok(())
}
