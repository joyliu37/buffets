#include <iostream>
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vbuffet.h"
using namespace std;
vluint64_t main_time = 0;
double sc_time_stamp () {// Called by $time in Verilog
    return main_time;// Note does conversion to real, to match SystemC
}

void dump_trace(VerilatedVcdC* tfp) {
    for (int i = 0; i < 5; i ++) {
       tfp->dump(main_time);
        main_time++;
    }
}
void reset(Vbuffet* dut, VerilatedVcdC* tfp) {

    dut->nreset_i = 0;
    dut->clk = 0;
    dut->eval();
    dump_trace(tfp);

    dut->clk = 1;
    dut->eval();
    dump_trace(tfp);

    dut->nreset_i = 1;
    dut->clk = 0;
    dut->eval();
    dump_trace(tfp);

    dut->clk = 1;
    dut->eval();
    dump_trace(tfp);
}

void init(Vbuffet* dut, VerilatedVcdC* tfp) {

    dut->clk = 0;
    dut->eval();
    dump_trace(tfp);

    dut->read_idx_valid = 0;
    dut->update_idx_valid = 0;
    dut->update_data_valid = 0;
    dut->push_data_valid = 0;
    dut->credit_ready= 0;
    dut->eval();
    dump_trace(tfp);

    dut->clk = 1;
    dut->eval();
    dump_trace(tfp);

    dut->clk = 0;
    dut->eval();
    dump_trace(tfp);

}
bool read_enable(Vbuffet* dut, int t, int valid_read_cnt, int valid_read_request) {
  cout  << "read idx ready: " << (int) dut->read_idx_ready << endl;
  //The II need to be one because the hardware is not optimized
  return (t > 1 /*&& t % 2 == 0*/) && (valid_read_request < 16) &&
      (dut->read_idx_ready) /*&& (valid_read_cnt >= valid_read_request)*/;
}

int main() {


    Vbuffet* dut = new Vbuffet;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open("obj_dir/sim_rate1.vcd");

    //reset
    reset(dut, tfp);
    //init
    init(dut, tfp);

    int valid_read_cnt = 0, valid_read_request = 0;
    //Fill in data and read
    for (int t = 0; t < 256; t++) {
        cout << "t = " << t << endl;
        //Verilated::timeInc(1);

        //Always check the read
        if (dut->read_data_valid) {
          unsigned int val((unsigned int) (dut->read_data));
          cout << "\tRead data [" << val <<"] at t = " << t << endl;
          valid_read_cnt ++;
        }
        dut->clk = 0;
        dut->eval();
        dump_trace(tfp);

        //if (t < 32 && (t%2 == 0)) {
        if (t < 16) {
          //Push data
          cout << "\tPush data " << t << endl;
          dut->push_data = (unsigned int)(t);
          dut->push_data_valid = 1;

        } else {
          dut->push_data_valid = 0;
        }
        if (read_enable(dut, t, valid_read_cnt, valid_read_request)) {
          cout << "\tSend read request for id = " << valid_read_request << endl;
          //send read request
          dut->read_idx = (valid_read_request);
          dut->read_idx_valid = 1;
          dut->is_shrink = 0;
          dut->read_will_update = 0;
          valid_read_request ++;
        } else {
          dut->read_idx_valid = 0;
        }
        dut->read_data_ready = 1;
        //make sure combinational propagate
        dut->eval();
        dut->clk = 1;
        dut->eval();
        dump_trace(tfp);
        /*
        dut->push_data_valid = 0;
        dut->clk = 0;
        dut->eval();
        dut->clk = 1;
        dut->eval();
        dut->clk = 0;
        dut->eval();
        dut->read_idx = (unsigned char) (valid_read_cnt);
        dut->read_idx_valid = 1;
        dut->read_data_ready = 1;
        dut->is_shrink = 0;
        dut->read_will_update = 0;
        dut->clk = 1;
        dut->eval();
        dut->clk = 0;
        dut->eval();
        dut->read_idx_valid = 0;
        int latency = 0;
        while (dut.read_data_valid == 0) {
            cout << "\t\t\tLatency: " << latency << endl;
            assert(latency < 1000);
            dut.clk = 1;
            dut.eval();
            cout << "\t\t\tread valid: " <<(int)  dut.read_idx_valid << endl;
            dut.clk = 0;
            dut.eval();
            cout << "\t\t\tread valid: " <<(int) dut.read_idx_valid << endl;
            latency ++;
        }
        valid_read_cnt ++;
        unsigned int val((unsigned int) (dut.read_data));
        cout << "Read data [" << val <<"] at t = " << t << endl;
        dut.clk = 1;
        dut.eval();
        if (valid_read_cnt >= 16)
            break;
        */
    }
    tfp->close();
    return 0;
}
