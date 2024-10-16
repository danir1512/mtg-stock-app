use std::ffi::CString;
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn fetch_web_page() -> *mut c_char {
    let url = "https://httpbin.org/get";
    let response_text = reqwest::blocking::get(url)
        .expect("Failed to fetch web page")
        .text()
        .expect("Failed to parse response");

    let c_string = CString::new(response_text).unwrap();
    c_string.into_raw()  // Return the C-compatible string pointer
}

#[no_mangle]
pub extern "C" fn free_string(s: *mut c_char) {
    if !s.is_null() {
        unsafe { let _ = CString::from_raw(s); }  // Frees the memory
    }
}