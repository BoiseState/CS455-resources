defmodule TimeClient.Client do
  require Logger

  def start_link(serverhost, port) do
    GenServer.start_link(__MODULE__, [:ok, serverhost, port], [])
  end

  def init([:ok, serverhost, port]) do
    Logger.info "Connecting to #{serverhost}:#{port}"
    case :gen_tcp.connect(to_char_list(serverhost), String.to_integer(port), [:binary, active: false, packet: :line]) do
      {:ok, server} ->
        Logger.info "Getting time"
        get_time(server)
      {:error, reason} ->
        Logger.info "Connection failed: REASON:  #{reason}"
    end

    {:ok, %{}}
  end

  def get_time(server) do
    :ok = :gen_tcp.send(server, "time\r\n")
    case :gen_tcp.recv(server, 0, 5000) do
      {:ok, data} ->
        IO.puts String.strip(data)
        read_line_from_server(server)
      {:error, :timeout} -> nil
      {:error, reason} -> Logger.info "Error receiving data. Reason: #{reason}"
    end
  end


  def read_line_from_server(socket) do
    case :gen_tcp.recv(socket, 0, 200) do
      {:ok, data} ->
        IO.puts String.strip(data)
        read_line_from_server(socket)
      {:error, :timeout} -> nil
      {:error, :closed} -> nil
      {:error, reason} -> Logger.info "Error receiving data. Reason: #{reason}"
    end
  end
end

