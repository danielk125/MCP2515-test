import can
import cantools


def convert_signal_type(signal):
    # float case
    if signal.is_float:
        if signal.length == 32:
            return "FLOAT"
        else:
            raise ValueError("Unrecognized type")
    if signal.choices:
        return "ENUM"
    # integer case
    if signal.is_signed:
        if signal.length <= 8:
            return "INT8"
        elif signal.length <= 16:
            return "INT16"
        elif signal.length <= 32:
            return "INT32"
        else:
            return "INT64"
    else:
        if signal.length <= 8:
            return "UINT8"
        elif signal.length <= 16:
            return "UINT16"
        elif signal.length <= 32:
            return "UINT32"
        else:
            return "UINT64"
    

def generate_signals(db_file, output_file):
    db = cantools.database.load_file(db_file)

    with open(output_file, 'w') as f:
        for message in db.messages:
            for signal in message.signals:
                signal_name = signal.name
                # Figure out underlying type
                signal_type = convert_signal_type(signal)
                signal_start_bit = signal.start
                signal_length = signal.length
                signal_factor = signal.factor
                signal_offset = signal.offset
                    
                f.write(f"CAN_Signal {signal_name}(SignalType::{signal_type}, {signal_start_bit}, {signal_length}, {signal_factor}, {signal_offset})\n")


generate_signals("can_database.dbc", "generated_signals.cpp")