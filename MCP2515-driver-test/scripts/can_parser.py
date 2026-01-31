import cantools.database
import cantools
import time

MANDATORY_COLUMNS = {
    "Message ID": "hex",
    "Message Name": "str",
    "Sender": "str",
    "Signal Name": "str",
    "Start Bit": "int",
    "Size (bits)": "int",
    "Factor": "float",
    "Offset": "float",
    "Min": "float",
    "Max": "float",
    "Unit": "str",
    "Cycle Time (ms)": "int",
    "Data Type": "str",
}


# this was a remnant from the original code that uses pandas
def get_column_name_from_index(index):
    return index 
    return list(MANDATORY_COLUMNS.keys())[index]


class CANSignal:
    @classmethod
    def create_from_valid_list(cls, row):
        """
        Create a CANSignal object from a row in a valid data frame. The row should be in the following format:
        Message ID, Message Name, Sender, Signal Name, Start Bit, Size (bits), Factor, Offset, Min, Max	Unit, Cycle Time (ms), Data Type
        """
        return cls(
            signal_name=row[get_column_name_from_index(3)],  # signal_name=row[3],
            start_bit=row[get_column_name_from_index(4)],  # start_bit=row[4],
            bit_length=row[get_column_name_from_index(5)],  # bit_length=row[5],
            factor=row[get_column_name_from_index(6)],  # factor=row[6],
            offset=row[get_column_name_from_index(7)],  # offset=row[7],
            data_min=row[get_column_name_from_index(8)],  # data_min=row[8],
            data_max=row[get_column_name_from_index(9)],  # data_max=row[9],
            unit=row[get_column_name_from_index(10)],  # unit=row[10],
            data_type=row[get_column_name_from_index(12)],  # data_type=row[12]
        )

    def __init__(
        self,
        signal_name,
        start_bit,
        bit_length,
        factor,
        offset,
        data_min,
        data_max,
        unit,
        data_type,
    ):
        self.signal_name = signal_name
        self.start_bit = start_bit
        self.bit_length = max(1, bit_length)
        self.factor = factor
        self.offset = offset
        self.data_min = data_min
        self.data_max = data_max
        self.unit = unit
        self.data_type = data_type
        self.message = None

    def get_cpp_signal_name(self, cpp_bus_name, cpp_message_name):
        cpp_name = f"{cpp_bus_name}_{cpp_message_name}_{self.signal_name}"
        # cleanup the signal name
        cpp_name = cpp_name.strip()
        # replace all non-alphanumeric characters with underscores
        cpp_name = "".join([c if c.isalnum() else "_" for c in cpp_name])
        # remove any leading underscores
        cpp_name = cpp_name.lstrip("_")
        # set the signal name to lowercase
        cpp_name = cpp_name.lower()

        return cpp_name

    def as_cpp_code(self):
        """
        Return the C++ code that represents this signal. The code should be in the following format:
        CANSignal using the MakeSignedCANSignal(SignalType, position, length, factor, offset) or
        MakeUnsignedCANSignal(SignalType, position, length, factor, offset) macro. There are no constructor arguments
        """
        # is the data_type unsigned?
        data_type = "unknown" if self.data_type == "" else self.data_type

        is_unsigned = data_type.startswith("u")

        macro = "MakeSignalSigned" if not is_unsigned else "MakeSignal"
        signal_name = self.signal_name.lower()
        if is_unsigned:
            return f"{macro}({data_type}, {self.start_bit}, {self.bit_length}, {self.factor}, {self.offset}) {signal_name};"
        else:
            return f"{macro}({data_type}, {self.start_bit}, {self.bit_length}, {self.factor}, {self.offset}, {not is_unsigned}) {signal_name};"

    def as_cantools_representation(self):
        """
        Return the cantools representation of this signal
        """
        # print("Creating cantools representation for signal ", self.signal_name)
        # print(self.signal_name, self.start_bit, self.bit_length, self.factor, self.offset, self.data_min, self.data_max, self.unit, self.data_type)
        signal = cantools.database.Signal(
            name=self.signal_name,
            start=self.start_bit,
            length=self.bit_length,
            minimum=self.data_min,
            maximum=self.data_max,
            unit=self.unit,
            is_signed=(not self.data_type.startswith("u")),
        )

        signal.offset = self.offset
        signal.scale = self.factor
        return signal
    
    def __str__(self):
        return self.signal_name
    
    def __dict__(self):
        return {
            "name": self.signal_name,
            "start_bit": self.start_bit,
            "bit_length": self.bit_length,
            "factor": self.factor,
            "offset": self.offset,
            "data_min": self.data_min,
            "data_max": self.data_max,
            "unit": self.unit,
            "data_type": self.data_type,
        }


class CANMessage:
    @classmethod
    def create_from_valid_list(cls, row):
        """
        Create a CANMessage object from a row in a valid data frame file. The row should be in the following format:
        Message ID, Message Name, Sender, Signal Name, Start Bit, Size (bits), Factor, Offset, Min, Max	Unit, Cycle Time (ms), Data Type
        """
        message = cls(
            message_id=row[get_column_name_from_index(0)],  # message_id=row[0],
            message_name=row[get_column_name_from_index(1)],  # message_name=row[1],
            sender=row[get_column_name_from_index(2)],  # sender=row[2],
            cycle_time=row[get_column_name_from_index(11)],  # cycle_time=row[11]
        )
        signal = CANSignal.create_from_valid_list(row)
        message.add_signal(signal)
        return message

    def __init__(self, message_id, message_name, sender, cycle_time):
        self.message_id = message_id
        self.message_name = message_name
        self.sender = sender
        self.cycle_time = cycle_time
        self.signals = []
        self.buses = set()

    def get_signals(self):
        return self.signals

    def add_signal(self, signal):
        if signal is None:
            return
        
        signal.message = self
        self.signals.append(signal)

    def message_id_as_integer(self):
        # message id should be a hex string that starts with '0x'
        # after the '0x', there should be only hex characters
        if self.message_id is None or not self.message_id.startswith("0x"):
            return 0xFF

        message_id = self.message_id[2:]
        try:
            return int(message_id, 16)
        except ValueError:
            return 0xEE
    
    def is_extended_id(self):
        message_id = self.message_id_as_integer()
        return message_id > 0x7FF
    
        
    
    def get_message_size_bytes(self):
        # find the end position of the highest signal
        end_position = max(
            [signal.start_bit + signal.bit_length for signal in self.signals]
        )

        # print("End position: ", end_position)

        message_size_bytes = end_position // 8 + (1 if end_position % 8 else 0)
        message_size_bytes = max(1, message_size_bytes)
        return message_size_bytes

    def get_cpp_message_name(self, cpp_bus_name):
        cpp_name = f"{cpp_bus_name}_{self.message_name}"
        # cleanup the message name
        cpp_name = cpp_name.strip()
        # replace all non-alphanumeric characters with underscores
        cpp_name = "".join([c if c.isalnum() else "_" for c in cpp_name])
        # remove any leading underscores
        cpp_name = cpp_name.lstrip("_")
        # set the message name to lowercase
        cpp_name = cpp_name.lower()
        return cpp_name

    def as_cpp_transmit_code(self, cpp_bus_name):
        """
        Return the C++ code that represents this message. The code should be in the following format:
        CANTXMessage takes the CAN bus to transmit on, the message ID, the message size in bytes (based on the end position
        of the highest signal), the transmit period, and the signals as arguments
        CANTXMessage can also take a VirtualTimerGroup to add its transmit timer to.
        For example:
        CANTXMessage<4> tx_message{
            can_bus, 0x100, 8, 100, timer_group, float_tx_signal, uint8_t_tx_signal, bool_tx_signal, millis_tx_signal};
        """

        cpp_code = "\n".join(
            [
                f"{signal.as_cpp_code(cpp_bus_name, self.message_name)}"
                for signal in self.signals
            ]
        )

        message_size_bytes = self.get_message_size_bytes()

        num_signals = len(self.signals)
        signal_list = ", ".join(
            [
                signal.get_cpp_signal_name(
                    cpp_bus_name, self.message_name
                )
                for signal in self.signals
            ]
        )

        # print(cpp_code)

        message_name = self.get_cpp_message_name(cpp_bus_name)
        message_code = f"CANTXMessage<{num_signals}> {message_name}{{{cpp_bus_name}, {self.message_id}, {message_size_bytes}, {self.cycle_time}, {signal_list}}};"
        # print(message_code)
        return cpp_code + "\n\n" + message_code

    def as_cpp_receive_code(self, cpp_bus_name):
        """
        Return the C++ code that represents this message. The code should be in the following format:
        CANRXMessage takes the CAN bus to receive on, the message ID, and the signals to be received as constructor arguments.
        CANRXMessages automatically register themselves with the can_bus on construction.
        For example:
        CANRXMessage<4> rx_message{can_bus,
                                   0x200,
                                   []() { Serial.println(rx_message.GetLastReceiveTime()); },
                                   float_rx_signal,
                                   uint8_t_rx_signal,
                                   bool_rx_signal,
                                   millis_rx_signal};
        """    
        cpp_code = "\n".join(
            [
                f"{signal.as_cpp_code(cpp_bus_name, self.message_name)}"
                for signal in self.signals
            ]
        )

        num_signals = len(self.signals)
        signal_list = ", ".join(
            [
                signal.get_cpp_signal_name(
                    cpp_bus_name, self.message_name
                )
                for signal in self.signals
            ]
        )

        # print(cpp_code)
        message_name = self.get_cpp_message_name(cpp_bus_name)
        message_code = f"CANRXMessage<{num_signals}> {message_name}{{{cpp_bus_name}, {self.message_id}, {signal_list}}};"
        # print(message_code)

        return cpp_code + "\n\n" + message_code

    def as_cantools_representation(self):
        """
        Return the cantools representation of this message
        """
        return cantools.database.Message(
            frame_id=self.message_id_as_integer(),
            name=self.message_name,
            senders=[self.sender],
            length=self.get_message_size_bytes(),
            signals=[
                signal.as_cantools_representation()
                for signal in sorted(self.signals, key=lambda x: x.start_bit)
            ],
            cycle_time=self.cycle_time,
        )
    
    def __str__(self):
        return self.message_name

    def __dict__(self):
        return {
            "id": self.message_id,
            "name": self.message_name,
            "sender": self.sender,
            "cycle_time": self.cycle_time,
            "bus"
            "signals": [
                signal.__dict__()
                for signal in sorted(self.signals, key=lambda x: x.start_bit)
            ],
            "transmit_code": self.as_cpp_transmit_code("can_bus"),
            "receive_code": self.as_cpp_receive_code("can_bus"),
        }
        

class CANBus:
    def __init__(self, bus_name):
        self.messages = set()
        self.name = bus_name

    def add_message(self, message):
        message.buses.add(self)
        self.messages.add(message)

    def get_cpp_bus_name(self):
        # cleanup the bus name
        cpp_bus_name = self.name.strip()
        # replace all non-alphanumeric characters with underscores
        cpp_bus_name = "".join([c if c.isalnum() else "_" for c in cpp_bus_name])
        # remove any leading underscores
        cpp_bus_name = cpp_bus_name.lstrip("_")
        # set the bus name to lowercase
        cpp_bus_name = cpp_bus_name.lower()

        return cpp_bus_name
    
    def as_cantools_representation(self) -> cantools.database.Database:
        """
        Return the cantools representation of this bus
        """
        # print("Creating cantools representation for bus ", self.name)
        database = cantools.database.Database(
            messages=[
                message.as_cantools_representation()
                for message in sorted(self.messages, key=lambda x: x.message_name)
            ]
        )
        return database
    
    def as_dbc_file(self):
        """
        Return the DBC file representation of this bus
        """
        database = self.as_cantools_representation()
        return database.as_dbc_string()
    
    def as_telem_file(self) -> str:
        """
        Emit the Telemetry Configuration Language, grouping boards by sender:

        > SenderName
        >> MessageName 0xID LengthBytes SenderName
        >>> SignalName dataType startBit bitLength factor offset
        """
        # 1) bucket messages by sender
        by_sender = {}
        for msg in sorted(self.messages, key=lambda m: m.message_name):
            sender = msg.sender or "UNKNOWN"
            by_sender.setdefault(sender, []).append(msg)

        # 2) build lines
        lines = []
        for sender in sorted(by_sender):
            lines.append(f"> {sender}")
            for msg in by_sender[sender]:
                mid   = msg.message_id_as_integer()
                size  = msg.get_message_size_bytes()
                # include sender on the message line
                lines.append(f">> {msg.message_name} 0x{mid:03X} {size} {sender}")
                # now each signal
                for sig in sorted(msg.signals, key=lambda s: s.start_bit):
                    dt = sig.data_type
                    sb = sig.start_bit
                    bl = sig.bit_length
                    f  = sig.factor
                    o  = sig.offset
                    lines.append(f">>> {sig.signal_name} {dt} {sb} {bl} {f} {o}")
        return "\n".join(lines)


    def __dict__(self):
        return {
            "name": self.name,
            "messages": [
                message.__dict__()
                for message in sorted(self.messages, key=lambda x: x.message_name)
            ],
        }

    def __str__(self):
        return self.name


class CANDatabase:
    def __init__(self, data):
        """
        Create a CANDatabase object from a list of lists. Each list should represent a row in a CSV file.
        """

        self.can_buses = []
        self.messages = {}  # message_id -> CANMessage
        self.signals = []
        self.load_database(data)

    

    def load_database(self, raw_data):
        res, data = self.validate_data(raw_data)
        if not res:
            raise ValueError(data)

        # Create the buses from the header row
        header = raw_data[0]

        if len(header) == len(MANDATORY_COLUMNS):
            self.can_buses.append(CANBus("Default"))
    
        for i, bus in enumerate(header[len(MANDATORY_COLUMNS):]):
            self.can_buses.append(CANBus(bus))


        # Process each row in the data
        for row in data:
            message_name = row[header.index("Message Name")]

            if message_name == "":
                continue

            if message_name not in self.messages.keys():
                self.messages[message_name] = CANMessage.create_from_valid_list(row)
            else:
                signal = CANSignal.create_from_valid_list(row)
                self.messages[message_name].add_signal(signal)
                self.signals.append(signal)

            # Get the last few bus columns, which specify which bus the message belongs to
            bus_columns = row[len(MANDATORY_COLUMNS):]
            print(bus_columns)

            if not any(bus_columns):
                # if no buses are specified, add the message to a default bus
                self.can_buses[0].add_message(self.messages[message_name])
            else:
                for i, bus in enumerate(bus_columns):
                    if bus:
                        print(f"Adding message {message_name} to bus {self.can_buses[i].name}")
                        self.can_buses[i].add_message(self.messages[message_name])


    def __dict__(self):
        return {"buses": [bus.__dict__() for bus in self.can_buses]}

    def get_buses(self):
        return self.can_buses

    def get_messages(self):
        return self.messages
    
    def get_bus_by_name(self, bus_name):
        for bus in self.can_buses:
            if bus.name == bus_name:
                return bus
        return None

    def validate_data(self, data):
        """
        Validate the CSV data to ensure that it has all the required columns.
        Returns a tuple (bool, data) where the bool is True if the data is valid, and False otherwise.
        If the data is invalid, the second element of the tuple is an error message.
        If the data is valid, the second element of the tuple is the processed data.
        """
        header = data[0]
        rows = data[1:]

        # Ensure header contains all mandatory columns
        columns = header
        error_messages = []

        for column_name, column_type in MANDATORY_COLUMNS.items():
            if column_name not in columns:
                error_messages.append(f"Column '{column_name}' is missing")
            else:
                casting_error_messages = []

                def int_cast(x):
                    if x == "":
                        return 0
                    try:
                        return int(x)
                    except ValueError:
                        casting_error_messages.append(
                            f"Column '{column_name}' should be of type 'int.' Got '{x}' instead"
                        )
                        return x

                def float_cast(x):
                    if x == "":
                        return 0.0
                    try:
                        return float(x)
                    except ValueError:
                        casting_error_messages.append(
                            f"Column '{column_name}' should be of type 'float.' Got '{x}' instead"
                        )
                        return x

                def str_cast(x):
                    x = x.strip()
                    x = "".join([c for c in x if c.isalnum() or c in " _-"])
                    return x

                def hex_cast(x):
                    if x == "":
                        return None
                    return x

                casting_functions = {
                    "int": int_cast,
                    "str": str_cast,
                    "float": float_cast,
                    "hex": hex_cast,
                }

                col_index = columns.index(column_name)

                for row in rows:
                    if row[col_index] != row[col_index]:
                        continue;
                    row[col_index] = casting_functions[column_type](row[col_index])
                    

                if casting_error_messages:
                    error_messages.extend(casting_error_messages)

        def bool_cast(x):
            if isinstance(x, bool):
                return x

            if x == "":
                return False
            
            if x.lower() in ["true", "t", "yes", "y"]:
                return True
            elif x.lower() in ["false", "f", "no", "n"]:
                return False
            else:
                error_messages.append(
                    f"Column '{column_name}' should be of type 'bool.' Got '{x}' instead"
                )
                return x
            
        # Validate optional columns
        for i in range(len(MANDATORY_COLUMNS), len(columns)):
            column_name = columns[i]
            col_index = i
            for row in rows:
                try:
                    row[col_index] = bool_cast(row[col_index])
                except ValueError:
                    error_messages.append(f"Column '{column_name}' should be of type 'bool'")

        if error_messages:
            return False, ", ".join(error_messages)

        return True, rows
