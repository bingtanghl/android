

function hook_java() {
    Java.perform(function () {
        let LibBili = Java.use("com.bilibili.nativelibrary.LibBili");
        LibBili["s"].implementation = function (sortedMap) {
            console.log('s is called' + ', ' + 'sortedMap: ' + sortedMap);
            let ret = this.s(sortedMap);
            var keyStr = ""
            var keys = sortedMap.keySet();
            var key_set = keys.iterator();
            while (key_set.hasNext()) {
                var key = key_set.next().toString();
                keyStr += "," + key
            }

            console.log("keyStr:", keyStr)
            console.log("value :", sortedMap.values().toArray());

            console.log("biliNative s(" + sortedMap + ") = " + ret);
            return ret;
        };
        LibBili["a"].implementation = function (str) {
            console.log('a is called' + ', ' + 'str: ' + str);
            let ret = this.a(str);
            console.log('a ret value is ' + ret);
            return ret;
        };



        LibBili["so"].overload('java.util.SortedMap', 'int', 'int').implementation = function (sortedMap, i2, i3) {
            console.log('so is called' + ', ' + 'sortedMap: ' + sortedMap + ', ' + 'i2: ' + i2 + ', ' + 'i3: ' + i3);
            let ret = this.so(sortedMap, i2, i3);
            var keyStr = ""
            var keys = sortedMap.keySet();
            var key_set = keys.iterator();
            while (key_set.hasNext()) {
                var key = key_set.next().toString();
                keyStr += "," + key
            }

            console.log("keyStr:", keyStr)
            console.log("value :", sortedMap.values().toArray());

            console.log('so ret value is ' + ret);
            return ret;
        };
    })
}



function call_sign() {
    Java.perform(function () {
        var TreeMap = Java.use("java.util.TreeMap");
        var treeMap = TreeMap.$new();
        treeMap.put("appkey", "1d8b6e7d45233436",)
        treeMap.put("build", "6180500");
        treeMap.put("c_locale", "zh-Hans_CN");
        treeMap.put("channel", "shenma069");
        treeMap.put("mobi_app", "android");
        treeMap.put("plat", "0");
        treeMap.put("platform", "android");
        treeMap.put("s_locale", "zh-Hans_CN");
        treeMap.put("splash_id", "");
        treeMap.put("ts", "1681719796");
        var js = { "appId": 1, "platform": 3, "version": "6.18.0", "abtest": "" }
        treeMap.put("statistics", JSON.stringify(js));
        treeMap.put("type", "1");
        treeMap.put("ver", "7028916702750116725");
        let LibBili = Java.use("com.bilibili.nativelibrary.LibBili");
        var retval = LibBili.s(treeMap);
        console.log("retval = ", retval)

    })

}


function hook_dlopen(){
        var dlopen = Module.findExportByName(null,"android_dlopen_ext");
        console.log("dlopen :",dlopen)
        if(dlopen){
            Interceptor.attach(dlopen,{
                onEnter:function(args){
                    var pathptr = args[0];
                    if(pathptr!==undefined && pathptr!==null){
                        var so_path = ptr(pathptr).readCString();
                        if(so_path.indexOf("libmsaoaidsec.so")>=0){
                            this.is_can_hook = true;
                            locate_init()
                        }
                        console.log("so_path :",so_path)
                    }
                },onLeave:function(retval){
                    if(this.is_can_hook){
                        hook_JNI_Load()
                    }

                }
            })

        }
}

function hook_JNI_Load(){
    let module = Process.findModuleByName("libmsaoaidsec.so")
    Interceptor.attach(module.base.add(0xC6DC + 1), {
        onEnter(args){
            console.log("call JNI_OnLoad")
        }
    })
}

function locate_init(){
    let secmodule = null;
    Interceptor.attach(Module.findExportByName(null,"__system_property_get"),{
        onEnter:function(args){
            // _system_property_get("ro.build.version.sdk", v1);
            var name = args[0];
            if(name!==undefined&&name!=null){
                name = ptr(name).readCString();
                if (name.indexOf("ro.build.version.sdk") >= 0) {
                    // 这是.init_proc刚开始执行的地方，是一个比较早的时机点
                    // do something
                    // hook_pthread_create()
                    bypass()
                }
            }
        },onLeave:function(retval){

        }
    })
}

function hook_pthread_create(){
    console.log("libmsaoaidsec.so --- " + Process.findModuleByName("libmsaoaidsec.so").base)
    Interceptor.attach(Module.findExportByName("libc.so", "pthread_create"),{
        onEnter(args){
            let func_addr = args[2]
            console.log("The thread function address is " + func_addr)
        }
    })
}


function nop(addr) {
    Memory.patchCode(ptr(addr), 4, code => {
        const cw = new ThumbWriter(code, { pc: ptr(addr) });
        cw.putNop();
        cw.putNop();
        cw.flush();
    });
}


//bypassfrida  
//对应的函数偏移分别是0x11129和0x10975
function bypass(){
    let module = Process.findModuleByName("libmsaoaidsec.so")
    nop(module.base.add(0x10AE4))
    nop(module.base.add(0x113F8))
}


function find_RegisterNatives(params) {
    let symbols = Module.enumerateSymbolsSync("libart.so");
    let addrRegisterNatives = null;
    for (let i = 0; i < symbols.length; i++) {
        let symbol = symbols[i];
        
        //_ZN3art3JNI15RegisterNativesEP7_JNIEnvP7_jclassPK15JNINativeMethodi
        if (symbol.name.indexOf("art") >= 0 &&
                symbol.name.indexOf("JNI") >= 0 && 
                symbol.name.indexOf("RegisterNatives") >= 0 && 
                symbol.name.indexOf("CheckJNI") < 0) {
            addrRegisterNatives = symbol.address;
            console.log("RegisterNatives is at ", symbol.address, symbol.name);
            hook_RegisterNatives(addrRegisterNatives)
        }
    }

}


//动态注册
function hook_RegisterNatives(addrRegisterNatives) {

    if (addrRegisterNatives != null) {
        Interceptor.attach(addrRegisterNatives, {
            onEnter: function (args) {
                console.log("[RegisterNatives] method_count:", args[3]);
                let java_class = args[1];
                let class_name = Java.vm.tryGetEnv().getClassName(java_class);
                //console.log(class_name);

                let methods_ptr = ptr(args[2]);

                let method_count = parseInt(args[3]);
                for (let i = 0; i < method_count; i++) {
                    let name_ptr = Memory.readPointer(methods_ptr.add(i * Process.pointerSize * 3));
                    let sig_ptr = Memory.readPointer(methods_ptr.add(i * Process.pointerSize * 3 + Process.pointerSize));
                    let fnPtr_ptr = Memory.readPointer(methods_ptr.add(i * Process.pointerSize * 3 + Process.pointerSize * 2));

                    let name = Memory.readCString(name_ptr);
                    let sig = Memory.readCString(sig_ptr);
                    let symbol = DebugSymbol.fromAddress(fnPtr_ptr)
                    console.log("[RegisterNatives] java_class:", class_name, "name:", name, "sig:", sig, "fnPtr:", fnPtr_ptr,  " fnOffset:", symbol, " callee:", DebugSymbol.fromAddress(this.returnAddress));
                }
            }
        });
    }
}


//字符串解密
function print_string(addr){
    var baseadr=Module.findBaseAddress("libbili.so")
    if(baseadr){
        var adr_37070=baseadr.add(addr);
        console.log(" adr_37070 :",ptr(adr_37070).readCString())
    }
}





function main() {
    hook_dlopen()
    // hook_java()
    // find_RegisterNatives()
}


setImmediate(main)



