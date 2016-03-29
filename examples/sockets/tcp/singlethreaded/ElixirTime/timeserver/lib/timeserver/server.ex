defmodule TimeServer.Server do
  require Logger

  def accept(port) do
    # The options below mean:
    #
    # 1. `:binary` - receives data as binaries (instead of lists)
    # 2. `packet: :line` - receives data line by line
    # 3. `active: false` - blocks on `:gen_tcp.recv/2` until data is available
    # 4. `reuseaddr: true` - allows us to reuse the address if the listener crashes
    #
    {:ok, socket} = :gen_tcp.listen(port,
                      [:binary, packet: :line, active: false, reuseaddr: true])
    Logger.info "Accepting connections on port #{port}"
    loop_acceptor(socket)
  end

  def loop_acceptor(socket) do
    {:ok, client} = :gen_tcp.accept(socket)
    task = Task.Supervisor.async(TimeServer.TaskSupervisor, fn -> serve(client) end)
    Task.await(task)
	# on onyx, the milliseconds are doubling. So five seconds ends up being 2500 milliseconds??
    Stream.timer(2500) |> Enum.to_list
    loop_acceptor(socket)
  end

  defp serve(socket) do
    case socket |> read_line() do
      :ok -> nil
      {:error, reason} -> Logger.info "Error in TimeServer.serve: #{reason}"
      _ -> serve(socket)
              end
  end

  defp read_line(socket) do
    case :gen_tcp.recv(socket, 0) do
      {:ok, data} ->
        write_line(socket)
      {:error, :closed} -> nil
    end
  end

  defp write_line(socket) do
    {{year, month, day}, {hour, min, sec}} = :calendar.universal_time_to_local_time(:calendar.universal_time())
    Logger.info "Server time: #{month}/#{day}/#{year}, #{hour}:#{min}:#{sec}"
    :gen_tcp.send(socket, "Server time: #{month}/#{day}/#{year}, #{hour}:#{min}:#{sec} \r\n")
    :gen_tcp.close(socket)
  end

end

