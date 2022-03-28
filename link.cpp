#include "link.h"
#include <math.h>
//#define SERIAL_DEBUG

double rangeA, rangeB, rangeC;
String datastr;

struct MyLink *init_link()
{
#ifdef SERIAL_DEBUG
    Serial.println("init_link");
#endif
    struct MyLink *p = (struct MyLink *)malloc(sizeof(struct MyLink));
    p->next = NULL;
    p->anchor_addr = 0;
    p->range[0] = 0.0;
    p->range[1] = 0.0;
    p->range[2] = 0.0;

    return p;
}

typedef struct Position {
    double x;
    double y;
} Position;

void add_link(struct MyLink *p, uint16_t addr)
{
#ifdef SERIAL_DEBUG
    Serial.println("add_link");
#endif
    struct MyLink *temp = p;
    //Find struct MyLink end
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    Serial.println("add_link:find struct MyLink end");
    //Create a anchor
    struct MyLink *a = (struct MyLink *)malloc(sizeof(struct MyLink));
    a->anchor_addr = addr;
    a->range[0] = 0.0;
    a->range[1] = 0.0;
    a->range[2] = 0.0;
    a->dbm = 0.0;
    a->next = NULL;

    //Add anchor to end of struct MyLink
    temp->next = a;

    return;
}

struct MyLink *find_link(struct MyLink *p, uint16_t addr)
{
#ifdef SERIAL_DEBUG
    Serial.println("find_link");
#endif
    if (addr == 0)
    {
        Serial.println("find_link:Input addr is 0");
        return NULL;
    }

    if (p->next == NULL)
    {
        Serial.println("find_link:Link is empty");
        return NULL;
    }

    struct MyLink *temp = p;
    //Find target struct MyLink or struct MyLink end
    while (temp->next != NULL)
    {
        temp = temp->next;
        if (temp->anchor_addr == addr)
        {
            // Serial.println("find_link:Find addr");
            return temp;
        }
    }

    Serial.println("find_link:Can't find addr");
    return NULL;
}

void fresh_link(struct MyLink *p, uint16_t addr, float range, float dbm)
{
#ifdef SERIAL_DEBUG
    Serial.println("fresh_link");
#endif
    struct MyLink *temp = find_link(p, addr);
    if (temp != NULL)
    {
        temp->range[2] = temp->range[1];
        temp->range[1] = temp->range[0];

        temp->range[0] = (range + temp->range[1] + temp->range[2]) / 3;
        temp->dbm = dbm;
        return;
    }
    else
    {
        Serial.println("fresh_link:Fresh fail");
        return;
    }
}

void print_link(struct MyLink *p)
{
#ifdef SERIAL_DEBUG
    Serial.println("print_link");
#endif
    struct MyLink *temp = p;

    while (temp->next != NULL)
    {
        //Serial.println("Dev %d:%d m", temp->next->anchor_addr, temp->next->range);
        Serial.println(temp->next->anchor_addr, HEX);
        Serial.println(temp->next->range[0]);
        Serial.println(temp->next->dbm);
        temp = temp->next;
    }

    return;
}

void delete_link(struct MyLink *p, uint16_t addr)
{
#ifdef SERIAL_DEBUG
    Serial.println("delete_link");
#endif
    if (addr == 0)
        return;

    struct MyLink *temp = p;
    while (temp->next != NULL)
    {
        if (temp->next->anchor_addr == addr)
        {
            struct MyLink *del = temp->next;
            temp->next = del->next;
            free(del);
            return;
        }
        temp = temp->next;
    }
    return;
}

void calculatePose(double range_A, double range_B, double range_C)
{
//    double x, y; //data
//    Position position_self = {0,0}; //koordinat anchor A
//    Position position_B = {1,0}; //koordinat anchor B
//    Position position_C = {1,1}; //koordinat anchor C
//    /*calculate pose using trilateration
//    https://math.stackexchange.com/questions/884807/find-x-location-using-3-known-x-y-location-using-trilateration
//    */ 
//    double A = ( (-2*position_self.x) + (2*position_B.x) );
//    double B = ( (-2*position_self.y) + (2*position_B.y) );
//    double C = (range_A*range_A) - (range_B*range_B) - (position_self.x*position_self.x) + (position_B.x*position_B.x) - (position_self.y*position_self.y) + (position_B.y*position_B.y);
//    double D = ( (-2*position_B.x) + (2*position_C.x) );
//    double E = ( (-2*position_B.y) + (2*position_C.y) );
//    double F = (range_B*range_B) - (range_C*range_C) - (position_B.x*position_B.x) + (position_C.x*position_C.x) - (position_B.y*position_B.y) + (position_C.y*position_C.y);
//    x = (C*E-F*B) / (E*A-B*D);
//    y = (C*D-A*F) / (B*D-A*E);
//    //konversi double to string
//    String x_str = String(x,6);
//    String y_str = String(y,6);
//    //concatenation string
//    datastr = x_str + "," + y_str; //beri (,) sebagai pemisah saat parsing 
//    WireSlave.print(datastr); //kirim datastr .c_str()
//    Serial.println(datastr);
}

void make_link_json(struct MyLink *p, String *s)
{
//    *s = "{\"links\":[";
    struct MyLink *temp = p;
    bool rangestate = false;
    while (temp->next != NULL)
    {
        
        Serial.print("Range A: ");
        Serial.println(rangeA);
        Serial.print("Range B: ");
        Serial.println(rangeB);
        Serial.print("Range C: ");
        Serial.println(rangeC);
        
        temp = temp->next;
        
//        char link_json[50];
//        sprintf(link_json, "{\"Anchor\":\"%X\",\"Range\":\"%.3f\"}", temp->anchor_addr, temp->range[0]);
//        *s += link_json;
//        
//        if (temp->next != NULL)
//        {
//            *s += ",";
//        }
//        

        if(temp->anchor_addr == 1)
        {
          rangeA = temp->range[0];
        }  
        
        else if(temp->anchor_addr == 2)
        {
          rangeB = temp->range[0];
          rangestate = true;
//          continue;
        }
        
        else if(temp->anchor_addr == 3)
        {
          rangeC = temp->range[0];
          rangestate = false;
//          continue;
        }

        if(rangeA != NULL && rangeB != NULL && rangeC != NULL)
        {
//          calculatePose(rangeA, rangeB, rangeC);
          double x, y; //data
          Position position_self = {0,0}; //koordinat anchor A
          Position position_B = {0,14.5}; //koordinat anchor B
          Position position_C = {14.5,14.5}; //koordinat anchor C
          /*calculate pose using trilateration
          https://math.stackexchange.com/questions/884807/find-x-location-using-3-known-x-y-location-using-trilateration
          */ 
          double A = ( (-2*position_self.x) + (2*position_B.x) );
          double B = ( (-2*position_self.y) + (2*position_B.y) );
          double C = (rangeA*rangeA) - (rangeB*rangeB) - (position_self.x*position_self.x) + (position_B.x*position_B.x) - (position_self.y*position_self.y) + (position_B.y*position_B.y);
          double D = ( (-2*position_B.x) + (2*position_C.x) );
          double E = ( (-2*position_B.y) + (2*position_C.y) );
          double F = (rangeB*rangeB) - (rangeC*rangeC) - (position_B.x*position_B.x) + (position_C.x*position_C.x) - (position_B.y*position_B.y) + (position_C.y*position_C.y);
          x = (C*E-F*B) / (E*A-B*D);
          y = (C*D-A*F) / (B*D-A*E);
          Serial.print("x: ");
          Serial.print(x);
          Serial.print("\ty: ");
          Serial.println(y);
          //konversi double to string
          String x_str = String(x,6);
          String y_str = String(y,6);
          //concatenation string
          *s = x_str + "," + y_str; //beri (,) sebagai pemisah saat parsing 
//          WireSlave.update();
          Serial.println(datastr);
        }
    }
//    *s += "]}";
//    Serial.println(*s);
}
