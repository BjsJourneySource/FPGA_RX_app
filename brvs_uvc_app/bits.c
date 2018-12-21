#include <bits.h>

void gen_bits_map(unsigned long *fap_map,int src_fps,int target_fps, int maxbits)
{
    int i;
    int weight_p =0,weight_n = 0;  //P:1   N:0
    int src_bits = src_fps;
    int target_bits  = target_fps;
    int pos_nc = 0;
    int weight_nc = 0;
    int temp_bits =0;

    bitmap_zero(fap_map, maxbits);
    if(target_bits ==(src_bits-target_bits)){
        weight_n = weight_p = 1;

    }else if((src_bits!=0)&&(target_bits!=0)&&(src_bits!=target_bits)){
        weight_p = src_bits/(src_bits - target_bits);
        weight_n = src_bits/target_bits;
    }else{
        bitmap_fill(fap_map, target_fps);
        return;
    }

    while(target_fps>0){
        weight_nc = 0;
        temp_bits = target_bits;
        for (i = weight_p;i>0;i--){
            set_bit(pos_nc, fap_map);
            pos_nc++;
            weight_nc++;
            temp_bits--;
            if (temp_bits==0){
                break;
            }
        }
        target_bits -= weight_nc; 
        temp_bits = src_bits - temp_bits + weight_nc;
        for (i=weight_n;i>0;i--){
            clear_bit(pos_nc, fap_map);
            pos_nc++;
            weight_nc++;
            temp_bits--;
            if (temp_bits==0){
                break;
            }
        }
        src_bits -= weight_nc;
        if(target_bits ==(src_bits-target_bits)){
            weight_n = weight_p = 1;

        }
        else if((src_bits!=0)&&(target_bits!=0)&&(src_bits!=target_bits)){
            weight_p = src_bits/(src_bits - target_bits);
            weight_n = src_bits/target_bits;
        }else{
            break;
        }
        if (pos_nc >= src_fps){ 
            break;
        }
    }
    
    while(!test_bit(0, fap_map)){
        shift_right_bit(1, fap_map, maxbits);
    }
}


